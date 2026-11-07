
#include "ControlPoint.h"

class IntQueue {

	private:

		struct Node {
			int value;
			Node* next;

			Node(int v) : value(v), next(nullptr) {}
		};

		Node* head = nullptr;
		Node* tail = nullptr;
		int size = 0;

	public:

		IntQueue() = default;

		// Prevent accidental shallow copies
		IntQueue(const IntQueue&) = delete;
		IntQueue& operator=(const IntQueue&) = delete;

		~IntQueue() {
			while (!Empty())
				Dequeue();
		}

		// Insert an element at the back
		void Enqueue(int value) {
			Node* node = new Node(value);

			if (Empty()) {
				head = tail = node;
			} else {
				tail->next = node;
				tail = node;
			}

			++size;
		}

		// Remove and return the front element
		int Dequeue() {
			if (Empty())
				throw std::out_of_range("Queue is empty");

			Node* oldHead = head;
			int value = oldHead->value;

			head = head->next;
			delete oldHead;

			if (head == nullptr)
				tail = nullptr;

			--size;
			return value;
		}

		// Return the front element without removing it
		int Front() const {
			if (Empty())
				throw std::out_of_range("Queue is empty");

			return head->value;
		}

		bool Empty() const {
			return head == nullptr;
		}

		int Size() const {
			return size;
		}
};


class PointArrayQueue{

	protected:

		ControlPoint3D* TotalPoints;

		IntQueue FreeIndexes = IntQueue();
		int FreeSpaces = 2;

		int size = 2;

	public:

		PointArrayQueue(){
			
			TotalPoints = new ControlPoint3D[2];
			FreeIndexes.Enqueue(0);
			FreeIndexes.Enqueue(1);

		};

		PointArrayQueue(const PointArrayQueue&) = delete;
		PointArrayQueue& operator=(const PointArrayQueue&) = delete;

		~PointArrayQueue() {delete[] TotalPoints;}
		
		void Append(ControlPoint3D& p){

			if (FreeSpaces == 0){

				ControlPoint3D* newTotalPoints = new ControlPoint3D[2 * size];

				for (int k = 0; k < size; k++){

					newTotalPoints[k] = TotalPoints[k];
					FreeIndexes.Enqueue(size + k);
				}
				
				//avoid a huge memory leak
				delete[] TotalPoints;

				TotalPoints = newTotalPoints;
				size *= 2;
				FreeSpaces = size;			
			}

			int nextAvaiableIdx = FreeIndexes.Dequeue();
			TotalPoints[nextAvaiableIdx] = p;
			FreeSpaces -= 1;

		}

		void RemovePoint(ControlPoint3D& p){

			bool FoundPoint = false;
			int k = 0;

			while ((k < size) && (!FoundPoint)){

				if ( (&TotalPoints[k] == &p) && (p.IsVisible())){
					FoundPoint = true;
				}
				else{
					k += 1;
				}
			}

			if (FoundPoint){

				FreeIndexes.Enqueue(k);
				p.MakeInvisible();
				FreeSpaces += 1;
			}

			
		}


		void Plot(){

			for (int k = 0; k < size; k++){
				TotalPoints[k].Plot();
			}

		}


};
