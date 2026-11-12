

class GraphicsElement{

	protected:

		bool transparent = false;
		bool selected = false;		

	public:

		virtual void Select() = 0;
		virtual void Unselect() = 0;

		virtual void MakeInvisible() = 0;
		virtual void MakeVisible() = 0;

		virtual bool IsSelected() = 0;
		virtual bool IsVisible() = 0;

		virtual void Plot() = 0;		


};