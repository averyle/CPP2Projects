#pragma once
#include <cstddef>
#include <csignal>
#include <array>
#include <memory>
#include <algorithm>
#include <optional>

using namespace std;

namespace my {
	template<typename T, size_t Count>
	class static_array {
	public:
		class iterator {
		public:
			iterator(size_t index, static_array<T, Count>& container)
				:index_{ index }, container_{ container } {}

			T& operator*() {
				return container_[ index_ ];
			}

			void operator++() {
				index_++;
			}

			bool operator!=(const iterator& other) {
				return index_ != other.index_;
			}

		private:
			size_t index_;
			static_array<T, Count> container_;
		};

		size_t size() { return Count; }

		T& operator[](size_t index) {
			if (index < Count) {
				return data_[index];
			}

			throw std::out_of_range("index out of range");
		}

		iterator begin() {
			return iterator(0, *this);
		}

		iterator end() {
			return iterator(Count, *this);
		}

	private:
		std::unique_ptr<T[]> data_ = std::make_unique<T[]>(Count);
	};

	template<typename T>
	class dynamic_array {
	public:
		using value_type = T;

		size_t size() { return size_; }
		bool empty() { return size_ == 0; }

		void insert(size_t index, const T& value) {
			if (size_ == capacity_) {
				expand();
			}

			size_t cursor = size_;
			size_t prev = cursor - 1;
			while (cursor > index) {
				data_[cursor] = data_[prev];
				--prev;
				++cursor;
			}

			data_[index] = value;
			++size_;
		}

		void erase(size_t index) {
			for (size_t next = index + 1; next < size_; ++next, ++index) {
				data_[index] = data_[next];
			}

			--size_;
		}
		void push_back(const T& value) {
			if (size_ == capacity_) {
				expand();
			}
			data_[size_] = value;
			size_++;
		}

		void pop_back() {
			size_--;
		}

		void clear() {
			size_ = 0;
		}

		T& operator[](size_t index) {
			if (index < size_) {
				return data_[index];
			}

			throw std::out_of_range("index out of range");
		}

		T& front() {
			return data_[0];
		}

		T& back() {
			return data_[size_ - 1];
		}
		
	private:
		static constexpr size_t InitialCapacity = 8;

		std::unique_ptr<T[]> data_ = std::make_unique<T[]>(InitialCapacity);
		size_t size_ = 0;
		size_t capacity_ = InitialCapacity;

		void expand() {
			auto new_capacity = capacity_ * 2;
			auto new_data = std::make_unique<T[]>(new_capacity);

			std::copy_n(data_.get(), size_, new_data.get());

			data_ = std::move(new_data);
			capacity_ = new_capacity;
		}
	};

	template<typename T>
	class array_stack {
	public:
		size_t size() { return data_.size(); }
		bool empty() { return data_.empty(); }
		void clear() { data_.clear(); }
		void push(const T& item) { data_.push_back(item); }
		void pop() { data_.pop_back(); }
		T& top() { return data_.back(); }
	private:
		my::dynamic_array<T> data_;
	};

	template<typename T>
	class array_queue {
	public:
		size_t size() { return data_.size(); }
		bool empty() { return data_.empty(); }
		void clear() { data_.clear(); }

		void push(const T& item) {
			data_.push_back(item);
		}

		void pop() {
			data_.erase(0);
		}

		void front() {
			return data_.front();
		}
	private:
		my::dynamic_array<T> data_;
	};

	template<typename T>
	class ring_buffer {
	public:
		size_t size() { return size_; }
		bool empty() { return size_ == 0; }

		void clear() {
			size_ = 0;
			front_index_ = std::nullopt;
			back_index_ = std::nullopt;
		}

		void push_back(const T& item) {
			if (size_ == capacity_) {
				expand_container();
			}

			if (size_ == 0) {
				front_index_ = std::make_optional(0);
				back_index_ = std::make_optional(0);
			}
			else {
				auto next_back_index = (back_index_.value() + 1) % capacity_;
				back_index_ = std::make_optional(next_back_index);
			}

			data_[back_index_.value()] = item;
			++size_;
		}

		void pop_front() {
			if (size_ == 0) {
				return;
			}

			if (size_ == 1) {
				front_index_ = std::nullopt;
				back_index_ = std::nullopt;
			}
			else {
				auto next_front_index = (front_index_.value() + 1) % capacity_;
				front_index_ = std::make_optional(next_front_index);
			}

			--size_;
		}

		T& front() {
			if (front_index_.has_value()) {
				return data_[front_index_.value()];
			}

			throw std::out_of_range("container is empty");
		}
	private:
		static constexpr size_t InitialCapacity = 8;

		std::unique_ptr<T[]> data_ = std::make_unique<T[]>(InitialCapacity);
		size_t size_ = 0;
		size_t capacity_ = InitialCapacity;
		std::optional<size_t> front_index_ = std::nullopt;
		std::optional<size_t> back_index_ = std::nullopt;

		void expand_container() {
			size_t new_capacity = capacity_ * 2;
			auto new_data = std::make_unique<T[]>(new_capacity);

			size_t insert_index = 0;
			for (size_t from = front_index_.value(); from < size_; ++from) {
				new_data[insert_index] = data_[from];
				++insert_index;
			}

			for (size_t from = 0; from < front_index_.value(); ++from) {
				new_data[insert_index] = data_[from];
				++insert_index;
			}

			data_ = std::move(new_data);
			front_index_ = std::make_optional(0);
			back_index_ = std::make_optional(size_ - 1);
		}

	};

	template<typename T>
	class ring_queue {
	public:
		size_t size() { return data_.size(); }
		bool empty() { return data_.empty(); }
		void clear() { return data_.clear(); }
		void push(const T& item) {
			data_.push_back(item);
		}
		void pop() {
			data_.pop_front();
		}
		T& front() {
			return data_.front();
		}
	private:
		my::ring_buffer<T> data_;
	};

	template<typename T>
	class linked_list {
	public:
		struct node {
			node(const T& value) : value_{ value } {}
			T value_;
			std::unique_ptr<node> next_uptr_ = nullptr;
		};

		class iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			T& operator*() {
				return node_ptr_->value_;
			}

			bool operator!=(const iterator& other) {
				return node_ptr_ != other.node_ptr_;
			}

			void operator++() {
				node_ptr_ = node_ptr_->next_uptr_;
			}
		private:
			node* node_ptr_ = nullptr;

			iterator(node* node_ptr) : node_ptr_{ node_ptr } {}
			friend class linked_list<T>;
		};

		iterator begin() {
			return iterator{ front_uptr_.get() };
		}

		iterator end() {
			return iterator{ nullptr };
		}

		size_t size() {
			size_t count = 0;
			for (node* cursor = front_uptr_.get(); cursor != nullptr; cursor = cursor->next_uptr_.get()) {
				++count;
			}
			return count;
		}

		bool empty() {
			return front_uptr_ == nullptr;
		}

		void clear() {
			while (front_uptr_ != nullptr) {
				pop_front();
			}
		}

		void push_front(const T& value) {
			auto new_node_uptr = std::make_unique<node>(value);
			new_node_uptr->next_uptr_ = std::move(front_uptr_);
			front_uptr_ = std::move(new_node_uptr);
		}

		void pop_front() {
			if (front_uptr_ != nullptr) {
				front_uptr_ = std::move(front_uptr_->next_uptr_);
			}
		}

		void insert_after(iterator nodeItr, const T& value) {
			if (nodeItr.node_ptr_ == nullptr) { return; }

			auto new_node_uptr = std::make_unique<node>(value);
			new_node_uptr->next_uptr_ = std::move(nodeItr.node_ptr->next_uptr_);
			nodeItr.node_ptr->next_uptr_ = std::move(new_node_uptr);

		}

		void erase_after(iterator nodeItr) {
			if (nodeItr.node_ptr_ == nullptr) { return; }

			if (nodeItr.node_ptr_->next_uptr_ == nullptr) { return; }
			nodeItr.node_ptr_->next_uptr_ = std::move(nodeItr.node_ptr->next_uptr_->next_uptr_);

		}

		T& front() {
			if (front_uptr_ != nullptr) {
				return front_uptr_->value_;
			}

			throw std::out_of_range("container is empty");
		}
	private:
		std::unique_ptr<node> front_uptr_ = nullptr;
	};

	template<typename T>
	class linked_stack {
	public:
		size_t size() { return data_.size(); }
		bool empty() { return data_.empty(); }
		void clear() { return data_.clear(); }
		void push(const T& item) {
			data_.push_front(item);
		}
		void pop() {
			data_.pop_front();
		}
		T& top() {
			return data_.front();
		}
	private:
		my::linked_list<T> data_;
	};

	template<typename T>
	class doubly_linked_list {
	public:
		struct node {
			node(const T& value) : value_{ value } {}
			T value_;
			std::unique_ptr<node> next_uptr_ = nullptr;
			node* prev_ptr_ = nullptr;
		};

		class iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			T& operator*() {
				return node_ptr_->value_;
			}

			bool operator!=(const iterator& other) {
				return node_ptr_ != other.node_ptr_;
			}

			void operator++() {
				node_ptr_ = node_ptr_->next_uptr_.get();
			}

			void operator--() {
				node_ptr_ = node_ptr_->prev_ptr_;
			}
		private:
			node* node_ptr_ = nullptr;

			iterator(node* node_ptr) : node_ptr_{ node_ptr } {}
			friend class doubly_linked_list<T>;
		};

		iterator begin() {
			return iterator{ front_uptr_.get() };
		}

		iterator end() {
			return iterator{ nullptr };
		}

		size_t size() {
			size_t count = 0;
			for (node* cursor = front_uptr_.get(); cursor != nullptr; cursor = cursor->next_uptr_.get()) {
				++count;
			}
			return count;
		}

		bool empty() {
			return front_uptr_ == nullptr;
		}

		void clear() {
			while (front_uptr_ != nullptr) {
				pop_front();
			}
		}

		void push_front(const T& value) {
			auto new_node = std::make_unique<node>(value);
			insert_front(std::move(new_node));
		}

		void push_back(const T& value) {
			auto new_node = std::make_unique<node>(value);
			insert_back(std::move(new_node));
		}

		void pop_front() {
			if (front_uptr_ == nullptr) { return; }

			front_uptr_ = std::move(front_uptr_->next_uptr_);

			if (front_uptr_ == nullptr) {
				back_ptr_ = nullptr;
			}
			else {
				front_uptr_->prev_ptr_ = nullptr;
			}
		}

		void pop_back() {
			if (back_ptr_ == nullptr) return;

			back_ptr_ = back_ptr_->prev_ptr_;

			if (back_ptr_ == nullptr) {
				front_uptr_ = nullptr;
			}
			else {
				back_ptr_->next_uptr_ = nullptr;
			}
		}

		void insert(iterator nodeItr, const T& value) {
			if (nodeItr.node_ptr_ == nullptr) { return; }
			auto node_uptr = std::make_unique<node>(value);

			if (nodeItr.node_ptr_ == front_uptr_.get()) {
				insert_front(std::move(node_uptr));
				return;
			}
			
			node_uptr->prev_ptr_ = nodeItr.node_ptr_->prev_ptr_;
			nodeItr.node_ptr_->prev_ptr_ = node_uptr.get();

			node_uptr->next_uptr_ = std::move(node_uptr->prev_ptr_->next_uptr_ );
			node_uptr->prev_ptr_->next_uptr_ = std::move(node_uptr);
		}

		void erase(iterator nodeItr) {
			if (nodeItr.node_ptr_ == nullptr) { return; }

			if (nodeItr.node_ptr_ == front_uptr_.get()) {
				pop_front();
				return;
			}

			if (nodeItr.node_ptr_ == back_ptr_) {
				pop_back();
				return;
			}

			nodeItr.node_ptr_->next_uptr_->prev_ptr_ = nodeItr.node_ptr_->prev_ptr_;
			nodeItr.node_ptr_->prev_ptr_->next_uptr_ = std::move(nodeItr.node_ptr_->next_uptr_);
		}

		T& front() {
			if (front_uptr_ != nullptr) {
				return front_uptr_->value_;
			}

			throw std::out_of_range("container is empty");
		}

		T& back() {
			if (back_ptr_ != nullptr) {
				return back_ptr_->value_;
			}

			throw std::out_of_range("container is empty");
		}
	private:
		std::unique_ptr<node> front_uptr_ = nullptr;
		node* back_ptr_ = nullptr;

		void insert_front(std::unique_ptr<node> node_uptr) {
			if (back_ptr_ == nullptr) {
				back_ptr_ = node_uptr.get();
			}

			if (front_uptr_ != nullptr) {
				front_uptr_->prev_ptr_ = node_uptr.get();
			}

			node_uptr->next_uptr_ = std::move(front_uptr_);
			front_uptr_ = std::move(node_uptr);
		}

		void insert_back(std::unique_ptr<node> node_uptr) {
			node_uptr->prev_ptr_ = back_ptr_;
			back_ptr_ = node_uptr.get();

			if (front_uptr_ == nullptr) {
				front_uptr_ = std::move(node_uptr);
			}
			else {
				node_uptr->prev_ptr_->next_uptr_ = std::move(node_uptr);
			}
		}
	};

	template<typename T>
	class linked_queue {
	public:
		size_t size() { return data_.size(); }
		bool empty() { return data_.empty(); }
		void clear() { return data_.clear(); }
		void push(const T& item) {
			data_.push_back(item);
		}
		void pop() {
			data_.pop_front();
		}
		T& front() {
			return data_.front();
		}
	private:
		my::doubly_linked_list<T> data_;
	};
}