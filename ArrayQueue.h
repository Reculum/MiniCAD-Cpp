
#pragma once
#include "ControlPoint.h"
#include <vector>

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

template <typename D>

class PointArrayQueue{

	protected:

		std::vector<D*> TotalPoints;

		IntQueue FreeIndexes = IntQueue();
		int FreeSpaces = 2;

		int size = 2;

	public:

		PointArrayQueue(){
			
			TotalPoints.resize(2, nullptr);

			FreeIndexes.Enqueue(0);
			FreeIndexes.Enqueue(1);

		};

		PointArrayQueue(const PointArrayQueue&) = delete;
		PointArrayQueue& operator=(const PointArrayQueue&) = delete;

		int getSize(){return size;}

		D& getItem(int k){return *TotalPoints[k];}

		void Append(D& p){

			if (FreeSpaces == 0){

				std::vector<D*> newTotalPoints;
				newTotalPoints.resize(2 * size, nullptr);

				for (int k = 0; k < size; k++){

					newTotalPoints[k] = TotalPoints[k];
					FreeIndexes.Enqueue(size + k);
				}

				TotalPoints = newTotalPoints;
				FreeSpaces = size;				
				size *= 2;			
			}

			int nextAvaiableIdx = FreeIndexes.Dequeue();

			//warning: this is a copy!
			TotalPoints[nextAvaiableIdx] = &p;
			FreeSpaces -= 1;

		}

		void RemoveItem(int k){

			D& p = getItem(k);

			if (!p.IsVisible()) return;

			p.MakeInvisible();
			p.Unselect();
			
			//avoid leaking memory
			delete &p;
			TotalPoints[k] = nullptr;

			FreeIndexes.Enqueue(k);
			FreeSpaces += 1;
		
		}
		


		void RemovePoint(D& p){

			bool FoundPoint = false;
			int k = 0;

			while ((k < size) && (!FoundPoint)){

				if ( (TotalPoints[k] == &p) && (p.IsVisible())){
					FoundPoint = true;
				}
				else{
					k += 1;
				}
			}

			if (FoundPoint){

				FreeIndexes.Enqueue(k);

				delete &p;
				TotalPoints[k] = nullptr;

				p.Unselect();
				p.MakeInvisible();
				FreeSpaces += 1;
			}

			
		}
		

		std::vector<int> FindSelectedPoints(){

			std::vector<int> indexesSelected;

			for(int k = 0; k < size; k++){

				if ( (TotalPoints[k] != nullptr) && (getItem(k).IsSelected()) ){
					indexesSelected.push_back(k);
				}
			}

			return indexesSelected;

		}

		void Plot(){

			for (int k = 0; k < size; k++){

				if (TotalPoints[k] != nullptr){
					TotalPoints[k] -> Plot();
				}				
			}

		}


	};
