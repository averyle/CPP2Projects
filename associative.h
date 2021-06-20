#pragma once

#include <array>
#include <vector>
#include <utility>
#include <memory>
#include <optional>

namespace my {
    template<typename T, size_t Size>
    class lookup_table {
    public:
        lookup_table() {
            for (int i = 0; i < Size; i++) {
                table_[i] = std::nullopt;
            }
        }

        void set(size_t index, const T& value) {
            table_[index] = std::make_optional(value);
        }

        void unset(size_t index) {
            table_[index] = std::nullopt;
        }

        std::optional<T> lookup(size_t index) {
            return table_[index];
        }

    private:
        std::array<std::optional<T>, Size> table_;
    };

    template<typename T>
    class chaining_hash_table {
    public:
        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using reference         = T&;
            
            T& operator*() {
                return *(bucket_itr_.value());
            }
            
            bool operator!=(const iterator& other) {
                return bucket_idx_ != other.bucket_idx_ || bucket_itr_ != other.bucket_itr_;
            }
            
            void operator++() {
                auto& currentBucket = container_.buckets_[bucket_idx_.value()];
                //get the next value in the current bucket index
                if (std::next(bucket_itr_.value()) != std::end(currentBucket)) {
                    bucket_itr_++;
                } else {
                //if iterator is at the end already, move onto the next nonempty bucket
                    for (size_t cursor = bucket_idx_.value() + 1; cursor < container_.buckets_; ++cursor) {
                        if (!container_.table_[cursor].empty()) {
                            bucket_itr_ = std::make_optional(std::begin(container_.table_[cursor]));
                            bucket_idx_ = std::make_optional(cursor);
                            return;
                        }
                    }
                    //if iterator is at the end value of the last bucket, change iterator to std::nullopt
                    bucket_idx_ = std::nullopt;
                    bucket_itr_ = std::nullopt;
                }
            }
            
        private:
            my::chaining_hash_table<T>& container_;
            std::optional<std::size_t> bucket_idx_;
            std::optional<typename std::vector<T>::iterator> bucket_itr_;
            
            iterator(chaining_hash_table& container, std::optional<std::size_t> bucket_idx,  std::optional<typename std::vector<T>::iterator> bucket_itr) :
            container_{ container }, bucket_idx_{ bucket_idx }, bucket_itr_{ bucket_itr } {}
            
            friend class chaining_hash_table<T>;
        };

        iterator begin() {
            for (size_t cursor = 0; cursor < buckets_; ++cursor) {
                if (!table_[cursor].empty()) {
                    return iterator{*this, std::make_optional(cursor), std::make_optional(std::begin(table_[cursor]))};
                }
            }
            
            return end();
        }

        iterator end() {
            return iterator{*this, std::nullopt, std::nullopt};
        }
        
        size_t size() { return size_; }

        bool empty() { return size_ == 0; }

        void clear() {
            for (int i = 0; i < buckets_; i++) {
                table_[i].clear();
            }
        }

        void insert(const T& item) {
            //check threshold -- expand / re-hash
            if (size_/buckets_ > 1.5) {
                auto new_buckets =  buckets_ * 2;
                size_ = 0;
                std::unique_ptr<Bucket[]> new_table = std::make_unique<Bucket[]>(new_buckets);
                
                for (size_t from = 0; from < buckets_; ++from) {
                    for (auto element: table_[from]) {
                        auto bucket_idx = std::hash<T>{}(element) % new_buckets;
                        new_table[bucket_idx].push_back(item);
                        ++size_;
                    }
                }
                
                buckets_ = new_buckets;
                table_ = std::move(new_table);
                insert(item);

            }
            else {
                auto bucket_idx = std::hash<T>{}(item) % buckets_;
                table_[bucket_idx].push_back(item);
                ++size_;
            }
        }

        bool contains(const T& item) {
            auto itr = find(item);
            auto endItr = end();
            if (itr != endItr) {
                std::cout << &itr << ' ' << &endItr;
                return true;
                
            }
            return false;
        }

        iterator find(const T& item) {
            auto bucket_idx = std::hash<T>{}(item) % buckets_;
            auto bucket_itr = std::find(std::begin(table_[bucket_idx]), std::end(table_[bucket_idx]), item);
            if (bucket_itr != std::end(table_[bucket_idx])) {
                return iterator{*this, std::make_optional(bucket_idx), std::make_optional(bucket_itr)};
            }

            return end();
            
        }

        void erase(iterator it) {
            table_[it.bucket_idx_.value()].erase(it.bucket_itr_.value());
            --size_;
        }

    private:
        using Bucket = std::vector<T>;
        static constexpr std::size_t InitialBucketCount = 64;

        std::unique_ptr<Bucket[]> table_ = std::make_unique<Bucket[]>(InitialBucketCount);
        std::size_t buckets_ = InitialBucketCount;
        std::size_t size_ = 0;
        
    };

template<typename T>
   class probing_hash_table{
   public:
       class iterator{
       public:
           using iterator_catergory = std::bidirectional_iterator_tag;
           using difference_type = std::ptrdiff_t;
           using value_type = T;
           using pointer = T*;
           using reference = T&;

           T& operator*(){
               auto& entry = container_.table_[index_].value();
               return std::get<T>(entry);
           }

           bool operator !=(const iterator& other){
               return index_ != other.index_;
           }

           void operator++(){
               ++index_;
               while (index_ < container_.capacity_ && container_.table_[index_] == std::nullopt){
                   ++index_;
               }
           }

           void operator--(){
               if (index_ > 0) { --index_;}

               while (index_ > 0 && container_.table_[index_] == std::nullopt){
                   --index_;
               }
           }

       private:
           probing_hash_table<T>& container_;
           std::size_t index_;

           iterator(probing_hash_table<T>& container, size_t index)
           : container_{ container }, index_{ index } {}

           friend class probing_hash_table<T>;
       };

       probing_hash_table(){
           reset();
       }

       iterator begin(){
           iterator it{ *this, 0 };

           while (table_[it.index_] == std::nullopt){
               ++it;
           }

           return it;
       }

       iterator end(){
           return iterator{ *this, capacity_};
       }

       size_t size(){ return size_; }

       bool empty(){ return size_ == 0; }

       void clear(){
           reset();
       }

       void insert(const T& item){
           if(size_ / capacity_ > 0.5){
               auto new_capacity = capacity_ * 2;
               size_ = 0;
               std::unique_ptr<Entry[]> new_table = std::make_unique<Entry[]>(new_capacity);

               for(size_t i = 0; i != capacity_; i++){
                   auto hash_index = std::hash<T>{}(table_[i].value().second) % new_capacity;
                   auto cursor_ = hash_index;
                   while (new_table[cursor_] != std::nullopt){
                       cursor_ = (cursor_ + 1) % capacity_;
                   }
                   table_[i].value().first = hash_index;
                   new_table[cursor_] = std::make_optional(table_[i].value());
                   ++size_;
               }

               table_ = std::move(new_table);
               capacity_ = new_capacity;
               insert(item);
           }

           else{
               auto hash_index = std::hash<T>{}(item) % capacity_;
               auto entry = std::make_pair(hash_index, item);

               auto cursor_ = hash_index;
               while (table_[cursor_] != std::nullopt){
                   cursor_ = (cursor_ + 1) % capacity_;
               }
               table_[cursor_] = std::make_optional(entry);
               ++size_;
           }
       }

       bool contains(const T& item){
           auto it = find(item);
           if(it != end()) { return true; }
           return false;
       }

       iterator find(const T& item){
           auto hash_index = std::hash<T>{}(item) % capacity_;

           size_t cursor_ = hash_index;
           while(table_[cursor_] != std::nullopt){
               auto [key, value] = table_[cursor_].value();
               if(key == hash_index && value == item){
                   return iterator{ *this, cursor_};
               }
               cursor_ = (cursor_ + 1) % capacity_;
           }
           return end();
       }

       void erase(iterator it){
           auto erase_index = it.index_;
           
           //loop through every index, starting from the one next to erase_index
           for (auto cursor = (erase_index + 1) % capacity_ ;; cursor = (cursor + 1) % capacity_) {
               
               //if element after erase index is empty, erase and return because there is nothing to shift
               if (table_[cursor] == std::nullopt) {
                   table_[erase_index] = std::nullopt;
                   --size_;
                   return;
               }
               
               //ideal index to put element in
               auto hash_index = std::hash<T>{}(table_[cursor].value().second) % capacity_;
               
               //distance from erase_index to ideal index
               auto erase_distance = std::abs(static_cast<int>(erase_index - hash_index));
               
               //distance from cursor to erase_index
               auto cursor_distance = std::abs(static_cast<int>(cursor - hash_index));
               
               if (erase_distance < cursor_distance) {
                   table_[erase_index] = table_[cursor];
                   
                   //advance erase index
                   erase_index = cursor;
               }
               
           }
       }
       
       friend std::ostream& operator<<(std::ostream& out, const probing_hash_table& table) {
           out << "[";
           for (int i = 0; i < table.capacity_; ++i) {
               if (table.table_[i].has_value()) {
                   out << i << ":(" << table.table_[i].value().first << "," << table.table_[i].value().second << ")";
               }
           }
           out << "]\n";
           return out;
       }

   private:
       using Entry = std::optional<std::pair<std::size_t, T>>;
       static constexpr std::size_t InitialCapacity = 64;

       std::unique_ptr<Entry[]> table_ = std::make_unique<Entry[]>(InitialCapacity);
       std::size_t capacity_ = InitialCapacity;
       std::size_t size_ = 0;

       void reset(){
           for (int i = 0; i < size_; ++i){
               table_[i] = std::nullopt;
           }
           size_= 0;
       }
       
   };

template<typename T>
class unordered_set {
public:
    size_t size() { return set.size(); }
    
    bool empty() { return set.empty(); }
    
    void clear() { set.clear(); }
    
    void insert(const T& item) {
        if (set.contains(item)) return;
        set.insert(item);
    }
    
    bool contains(const T& item) {
        return set.contains(item);
    }
    
    void erase(const T& item) {
        auto item_ = set.find(item);
        set.erase(item_);
    }
private:
    my::chaining_hash_table<T> set;
};

template <typename Key, typename Value>
struct KeyValuePair
{
    bool operator==(const KeyValuePair<Key, Value>& other) const {
        return other.key == key;
    }
    
  Key key;
  Value value;
};

template<typename Key, typename Value>
class unordered_map {
public:

    size_t size() { return map.size(); }

    bool empty() { return map.empty(); }

    void clear() { map.clear(); }

    void set(const Key& key, const Value& value) {
        if (map.contains(KeyValuePair<Key, Value> {key,value} )) {
            auto it = map.find(KeyValuePair<Key, Value> {key,value});
            (*it).value = value;
        }
        else {
            auto entry = KeyValuePair<Key, Value> {key,value};
            map.insert(entry);
        }
    }

    bool contains(const Key& key) {
        return (map.contains(KeyValuePair<Key, Value> {key,Value{}} ));
    }

    Value& get(const Key& key) {
        auto it = map.find(KeyValuePair<Key, Value> {key, Value{}});
        return (*it).value;
    }
    
    friend std::ostream& operator<<(std::ostream& out, unordered_map& other) {
        out << "[";
        for (auto i = other.map.begin(); i != other.map.end(); ++i) {
            out << (*i).key << "," << (*i).value << "|";
        }
        out << "]\n";
        return out;
    }
private:
    my::probing_hash_table<KeyValuePair<Key, Value>> map;
    size_t capacity_ = 64;
};
}

namespace std
{
    template<typename Key, typename Value> struct hash<my::KeyValuePair<Key, Value>>
    {
        std::size_t operator()(my::KeyValuePair<Key, Value> const& s) const noexcept
        {
            return std::hash<Key>{}(s.key);
        }
    };
}
