#pragma once

#include "ArrayQueue.h"
#include "GraphicsElement.h"
#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

class NURBS_Surface : public GraphicsElement {

protected:

    std::vector<ControlPoint3D*> CtrlPointsPointers;
    std::vector<Vector3> Tessellation;

    std::vector<float> NodesU;
    std::vector<float> NodesV;
    std::vector<float> weights;

    int controlPointsU = 0;
    int controlPointsV = 0;
    int degreeU = 0;
    int degreeV = 0;

    int tessellationU = 32;
    int tessellationV = 32;

	float ambientLight = 0.8f;
	float diffuseLight = 0.2f;
	Vector3 CeilingRay = {0.0f, 0.0f, 1.0f};

    int TotTimesUpdated = 0;

    static std::vector<float> BuildOpenUniformKnotVector(
        int numberOfControlPoints, int degree) {

        const int n = numberOfControlPoints - 1;
        const int m = n + degree + 1;

        std::vector<float> knots;
        knots.reserve(m + 1);

        for (int k = 0; k <= degree; ++k) {
            knots.push_back(0.0f);
        }

        for (int k = degree + 1; k <= n; ++k) {
            knots.push_back(
                static_cast<float>(k - degree) /
                static_cast<float>(n - degree + 1)
            );
        }

        for (int k = n + 1; k <= m; ++k) {
            knots.push_back(1.0f);
        }

        return knots;
    }

    // Returns the knot-span index. The endpoint u == 1 is handled explicitly.
    static int FindSpan(float u, int numberOfControlPoints, int degree,
                        const std::vector<float>& knots) {

        const int n = numberOfControlPoints - 1;

        if (u >= knots[n + 1]) {
            return n;
        }

        if (u <= knots[degree]) {
            return degree;
        }

        int low = degree;
        int high = n + 1;
        int mid = (low + high) / 2;

        while (u < knots[mid] || u >= knots[mid + 1]) {
            if (u < knots[mid]) {
                high = mid;
            }
            else {
                low = mid;
            }

            mid = (low + high) / 2;
        }

        return mid;
    }

    // Cox-de Boor basis values for the degree+1 nonzero basis functions
    // associated with the supplied knot span.
    static std::vector<float> BasisFunctions(
        int span, float u, int degree, const std::vector<float>& knots) {

        std::vector<float> N(degree + 1, 0.0f);
        std::vector<float> left(degree + 1, 0.0f);
        std::vector<float> right(degree + 1, 0.0f);

        N[0] = 1.0f;

        for (int j = 1; j <= degree; ++j) {
            left[j] = u - knots[span + 1 - j];
            right[j] = knots[span + j] - u;

            float saved = 0.0f;

            for (int r = 0; r < j; ++r) {
                const float denominator = right[r + 1] + left[j - r];
                const float temp = (denominator != 0.0f)
                    ? N[r] / denominator
                    : 0.0f;

                N[r] = saved + right[r + 1] * temp;
                saved = left[j - r] * temp;
            }

            N[j] = saved;
        }

        return N;
    }

    std::size_t ControlPointIndex(int i, int j) const {
        return static_cast<std::size_t>(i) *
               static_cast<std::size_t>(controlPointsV) +
               static_cast<std::size_t>(j);
    }

    std::size_t TessellationIndex(int i, int j) const {
        return static_cast<std::size_t>(i) *
               static_cast<std::size_t>(tessellationV + 1) +
               static_cast<std::size_t>(j);
    }

    void ComputeTessellation() {

        Tessellation.clear();
        Tessellation.reserve(
            static_cast<std::size_t>(tessellationU + 1) *
            static_cast<std::size_t>(tessellationV + 1)
        );

        for (int i = 0; i <= tessellationU; ++i) {
            const float u = static_cast<float>(i) /
                            static_cast<float>(tessellationU);

            for (int j = 0; j <= tessellationV; ++j) {
                const float v = static_cast<float>(j) /
                                static_cast<float>(tessellationV);

                Tessellation.push_back(Eval(u, v));
            }
        }
    }

public:

    NURBS_Surface() = default;

    NURBS_Surface(PointArrayQueue<ControlPoint3D>& ActivePoints, int numberOfControlPointsU, int numberOfControlPointsV, int p, int q,
		int samplesU = 32,
        int samplesV = 32
    ) {

        if (numberOfControlPointsU <= 0 || numberOfControlPointsV <= 0 ||
            p < 0 || q < 0 ||
            numberOfControlPointsU < p + 1 ||
            numberOfControlPointsV < q + 1) {
            throw std::invalid_argument(
                "Invalid surface dimensions or degrees"
            );
        }

        if (samplesU <= 0 || samplesV <= 0) {
            throw std::invalid_argument(
                "Tessellation sample counts must be positive"
            );
        }

        const std::vector<int> selectedPoints = ActivePoints.FindSelectedPoints();
        const std::size_t expectedNumberOfPoints =
            static_cast<std::size_t>(numberOfControlPointsU) *
            static_cast<std::size_t>(numberOfControlPointsV);

        if (selectedPoints.size() != expectedNumberOfPoints) {
            throw std::invalid_argument(
                "The number of selected control points must equal "
                "numberOfControlPointsU * numberOfControlPointsV"
            );
        }

        controlPointsU = numberOfControlPointsU;
        controlPointsV = numberOfControlPointsV;
        degreeU = p;
        degreeV = q;
        tessellationU = samplesU;
        tessellationV = samplesV;

        transparent = false;
        selected = false;

        CtrlPointsPointers.reserve(expectedNumberOfPoints);
        weights.resize(expectedNumberOfPoints, 1.0f);

        for (int selectedIndex : selectedPoints) {
            CtrlPointsPointers.push_back(&ActivePoints.getItem(selectedIndex));
        }

        NodesU = BuildOpenUniformKnotVector(controlPointsU, degreeU);
        NodesV = BuildOpenUniformKnotVector(controlPointsV, degreeV);

        // Compute the initial surface before marking the points as being used.
        ComputeTessellation();

        for (ControlPoint3D* point : CtrlPointsPointers) {
            point->MarkAsBelongingToCurve();
            TotTimesUpdated += point->getTimesUpdated();
        }
    }

    int getDegreeU() const { return degreeU; }
    int getDegreeV() const { return degreeV; }

    int getControlPointsU() const { return controlPointsU; }
    int getControlPointsV() const { return controlPointsV; }

    std::vector<float> getWeights() const { return weights; }
    std::vector<Vector3> getTessellation() const { return Tessellation; }
    std::vector<ControlPoint3D*> getCtrlPoints() const {
        return CtrlPointsPointers;
    }

    // The same flattened row-major indexing is used for weights and
    // control points: index = i * controlPointsV + j.
    void setWeight(float w, int i, int j) {

        if (i < 0 || i >= controlPointsU || j < 0 || j >= controlPointsV) {
            throw std::out_of_range("NURBS surface control-point index out of range");
        }

        if (!std::isfinite(w) || w <= 0.0f) {
            throw std::invalid_argument(
                "NURBS weights must be finite and strictly positive"
            );
        }

        weights[ControlPointIndex(i, j)] = w;
    }

    Vector3 Eval(float u, float v) const {

        if (!std::isfinite(u) || !std::isfinite(v) ||
            u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f) {
            throw std::invalid_argument(
                "NURBS surface parameters must lie in [0,1]"
            );
        }

        const int spanU = FindSpan(u, controlPointsU, degreeU, NodesU);
        const int spanV = FindSpan(v, controlPointsV, degreeV, NodesV);

        const std::vector<float> basisU =
            BasisFunctions(spanU, u, degreeU, NodesU);
        const std::vector<float> basisV =
            BasisFunctions(spanV, v, degreeV, NodesV);

        Vector3 numerator = {0.0f, 0.0f, 0.0f};
        float denominator = 0.0f;

        for (int a = 0; a <= degreeU; ++a) {
            const int i = spanU - degreeU + a;

            for (int b = 0; b <= degreeV; ++b) {
                const int j = spanV - degreeV + b;
                const std::size_t index = ControlPointIndex(i, j);

                const float coefficient =
                    basisU[a] * basisV[b] * weights[index];

                numerator = Vector3Add(
                    numerator,
                    Vector3Scale(CtrlPointsPointers[index]->getCoords(), coefficient)
                );

                denominator += coefficient;
            }
        }

        if (!std::isfinite(denominator) ||
            std::fabs(denominator) <= std::numeric_limits<float>::epsilon()) {
            throw std::runtime_error(
                "Cannot evaluate NURBS surface: rational denominator is zero"
            );
        }

        return Vector3Scale(numerator, 1.0f / denominator);
    }

    void Select() override { selected = true; }
    void Unselect() override { selected = false; }
    bool IsSelected() override { return selected; }

    void MakeInvisible() override { transparent = true; }
    void MakeVisible() override { transparent = false; }
    bool IsVisible() override { return !transparent; }

    void Plot() override {

        if (transparent) {
            return;
        }

        int currentTimesUpdated = 0;

        for (ControlPoint3D* point : CtrlPointsPointers) {
            currentTimesUpdated += point->getTimesUpdated();
        }

        if (currentTimesUpdated != TotTimesUpdated) {
            TotTimesUpdated = currentTimesUpdated;
            ComputeTessellation();
        }

        const Color surfaceColor = selected ? RED : GREEN;

        // Draw each quadrilateral cell as two triangles.
        for (int i = 0; i < tessellationU; ++i) {
            for (int j = 0; j < tessellationV; ++j) {

                const Vector3& p00 = Tessellation[TessellationIndex(i, j)];
                const Vector3& p10 = Tessellation[TessellationIndex(i + 1, j)];
                const Vector3& p11 = Tessellation[TessellationIndex(i + 1, j + 1)];
                const Vector3& p01 = Tessellation[TessellationIndex(i, j + 1)];

				Vector3 du = p10 - p00;
				Vector3 dv = p01 - p00;

				Vector3 norm = Vector3Normalize(Vector3CrossProduct(du, dv));

				float brightness = ambientLight + diffuseLight * Vector3DotProduct(norm, CeilingRay);

				Color surfaceColorShaded = ColorBrightness(surfaceColor, brightness); 


                DrawTriangle3D(p00, p10, p11, surfaceColorShaded);
                DrawTriangle3D(p00, p11, p01, surfaceColorShaded);
            }
        }

        // Draw the control net over the surface.
        for (int i = 0; i < controlPointsU; ++i) {
            for (int j = 0; j < controlPointsV; ++j) {
                const Vector3 point =
                    CtrlPointsPointers[ControlPointIndex(i, j)]->getCoords();

                if (i + 1 < controlPointsU) {
                    const Vector3 nextU =
                        CtrlPointsPointers[ControlPointIndex(i + 1, j)]->getCoords();
                    DrawLine3D(point, nextU, BLACK);
                }

                if (j + 1 < controlPointsV) {
                    const Vector3 nextV =
                        CtrlPointsPointers[ControlPointIndex(i, j + 1)]->getCoords();
                    DrawLine3D(point, nextV, BLACK);
                }
            }
        }
    }
};
