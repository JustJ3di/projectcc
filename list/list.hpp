#pragma once

template<typename T>
class list
{
private:
    class Node;
    class iterator;
    using Node_ptr = Node*;
    
    // Add the head pointer for the list
    Node_ptr head_ = nullptr; 
    
public:
    // ... Constructors and Destructor ...
    list()=default;
    ~list(){
        Node_ptr current_ = head_;
        while (current_)
        {
            Node_ptr temp = current_->get_next();
            delete current_;
            current_ = temp;
        }
        
    }

    /*Iterator methods*/
    iterator begin() {
        // 'begin' points to the first node
        return iterator(head_); 
    }

    iterator end() {
        // 'end' points to one-past-the-last element, which is nullptr
        return iterator(nullptr); 
    }

    void push_front(const T& data){
        Node_ptr new_node = new Node(data);
        new_node->set_next(head_);
        head_ = new_node;
    }


    
};


template<typename T>
class list<T>::Node{

public:
    /*Constructor*/
    Node(T data):next_(),data_(data){}

    Node_ptr get_next()const{return next_;}
    T& get_data(){return data_;}
    void set_next(Node_ptr x){next_ = x;}
    void set_data(){}


private:
    Node_ptr next_;
    T data_;
};

template<typename T>
class list<T>::iterator {
    
    // Allow the list to create iterators using the private Node*
    friend class list<T>; 

private:
    // A type alias for a pointer to a Node
    using Node_ptr = Node*;
    Node_ptr current_;

    // Private constructor: only list can create iterators directly from a Node*
    iterator(Node_ptr p) : current_(p) {}

public:

    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;

    // Default constructor for an invalid iterator
    iterator() : current_(nullptr) {}

// The dereference operator returns a reference to the data
    reference operator*() const {
        return current_->get_data(); 

    }

    // The arrow operator (used like: iter->method())
    pointer operator->() const {
        return &(operator*());
    }

// Pre-increment: ++iter
    iterator& operator++() {
        current_ = current_->get_next(); // Move to the next node
        return *this;
    }

    // Post-increment: iter++
    iterator operator++(int) {
        iterator tmp = *this; // Save the current state
        ++(*this);            // Perform the pre-increment
        return tmp;           // Return the saved state
    }

// Equality
    bool operator==(const iterator& other) const {
        return current_ == other.current_;
    }

    // Inequality (usually just the opposite of equality)
    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }


    
};

