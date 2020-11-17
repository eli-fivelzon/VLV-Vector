/**
 * @author Eli Fivelzon, eli.fivelzon@mail.huji.ac.il
 */
#ifndef VLVECTOR_HPP
#define VLVECTOR_HPP
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <stdexcept>

/**
 * Default static capacity in template.
 */
#define DEFAULT_STATIC_CAPACITY 16

/**
 * The growth rate of the memory if needed.
 */
#define GROWTH_FACTOR (3.0 / 2.0)

/**
 * Error message for execption in case of out of range index access.
 */
#define OUT_OF_RANGE_ERR_MSG "Index out of range."

/**
 * @class VLVector: A generic container similar to std::vector in interface
 * but manage smartly between stack and heap memory.
 * As long as stack memory is enough, it will be used, then if needed
 * it will appropriately to GROWTH_FACTOR, and if memory is less than
 * stack capacity, the stack will be used again.
 * @tparam T: the type of stored data.
 * @tparam StaticCapacity: The size of stack mem.
 */
template<class T, std::size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
private:
	T _staticMem[StaticCapacity];
	T* _data;
	std::size_t _size, _capacity;

	/**
	 * Copy all member fields smartly from other to this.
	 * Old members of this need to be taken care before.
	 * @param other
	 */
	void _copyMembers(const VLVector &other);

	/**
	 * Calculate new capacity based on CAPc formula for current size + additional size.
	 * @param additionalSize: The num of elements that need to be added.
	 * @return
	 */
	std::size_t _cap(std::size_t additionalSize = 1);

	/**
	 * Push some elements somewhere in the vector while making sure to resize mem if needed.
	 * This is done in one function to avoid redundant copying in case of need to copy
	 * to new memory and then to move some of the data to make space for new elements.
	 * Shift elements (if there are) from pos to the right by size, then call pushFunc.
	 * @tparam PushFunc: The type of insertion function.
	 * @param pos: The position in which to push the elements (will be inserted before pos).
	 * if pos == end then no shifting will take place.
	 * @param numOfElements: The number of new inserted elements.
	 * @param pushElements: A function that takes no args an will perform the insertion
	 * of new elements.
	 */
	template<class PushFunc>
	void _pushAt(const T *pos, std::size_t numOfElements, PushFunc pushElements);
public:
	/**
	 * @typedef iterator: def T* as iterator since it satisfies all requirements
	 * for random access iterator.
	 */
	typedef T* iterator;

	/**
	 * @typedef const_iterator: def const T* as const_iterator since
	 * it satisfies all requirements from const random access iterator.
	 */
	typedef const T*  const_iterator;

	/**
	 * Insert elements in the range [first, last) before pos.
	 * @tparam InputIterator: Type of first, last, must be at least input iterator.
	 * @param pos:
	 * @param first
	 * @param last:
	 * @return: Iterator to the first element inserted, or pos if no elements
	 * were inserted.
	 */
	template<class InputIterator>
	iterator insert(const_iterator pos, InputIterator first, InputIterator last);

	/**
	 * Default constructor, create an empty VLVector.
	 */
	VLVector(): _data(_staticMem), _size(0), _capacity(StaticCapacity) {}

	/**
	 * Copy constructor. Create a new VLVector identical to other.
	 * @param other
	 */
	VLVector(VLVector& other): VLVector() {_copyMembers(other); }

	/**
	 * Construtor from another range. Has the same effect as creating an empty VLVector,
	 * then performing insert(begin, first, last).
	 * @tparam InputIterator: Type of the input iterator to copy values from.
	 * @param first
	 * @param last
	 */
	template<class InputIterator>
	VLVector(InputIterator first, InputIterator last): VLVector() {insert(begin(), first, last); }

	/**
	 * Destructor. Free memory if needed.
	 */
	~VLVector();

	/**
	 * @return number of elements in VLVec.
	 */
	std::size_t size() const { return _size; }

	/**
	 * @return true if empty, false otherwise.
	 */
	bool empty() const { return _size == 0; }

	/**
	 * @return numer of elements that can be stored in current mem.
	 */
	std::size_t capacity() const { return _capacity; }

	/**
	 * Get value at idx with bound checking.
	 * @param index
	 * @return
	 */
	T& at(std::size_t index);

	/**
 * Get value at idx with bound checking for const.
 * @param index
 * @return
 */
	const T& at(std::size_t index) const;

	/**
	 * Insert value before pos.
	 * @param pos
	 * @param value
	 * @return Iterator pointing to the newly inserted value.
	 */
	iterator insert(const_iterator pos, const T& value);

	/**
	 * push value in the end of vector.
	 * @param value
	 */
	void push_back(const T& value) {_pushAt(cend(), 1, [&]() { _data[_size] = value; }); }

	/**
	 * Remove last value from vector.
	 */
	void pop_back() { erase(cend() -  1); }

	/**
	 * Erase elements in the range [first, last)
	 * @param first
	 * @param last
	 * @return Iterator pointing to the element after the last removed one.
	 * if first == last,return first.
	 */
	iterator erase(const_iterator first, const_iterator last);

	/**
	 * erase element at pos.
	 * @param pos
	 * @return: Iterator pointing after the removed element.
	 */
	iterator erase(const_iterator pos) { return erase(pos, pos + 1); }

	/**
	 * Remove all elements from vec.
	 */
	void clear();

	/**
	 * @return A pointer to the memory containing the data.
	 */
	T* data() { return _data; }


	/**
	 * @return A const pointer to the memory containing the data (const).
	 */
	const T* data() const { return _data; }

	/**
	 * Assignment operator
	 * @param other
	 * @return : this after assigment.
	 */
	VLVector& operator=(const VLVector& other);

	/**
	 * Access at idx [non-const], no bound checking.
	 * @param idx
	 * @return Value at idx by ref.
	 */
	T& operator[](std::size_t idx) { return _data[idx]; }

	/**
 * Access at idx [const], no bound checking.
 * @param idx
 * @return Value at idx by cosnt ref.
 */
	const T& operator[](std::size_t idx) const { return _data[idx]; }

	/**
	 * @param other
	 * @return true if this == other elementwise.
	 */
	bool operator==(const VLVector& other) const;

	/**
	 * @param other
	 * @return negation of operator==.
	 */
	bool operator!=(const VLVector& other) const { return !operator=(other); }

	/**
	 * @return iterator pointing to first element.
	 */
	iterator begin() { return _data; }

	/**
	 * @return iterator pointing after last element.
	 */
	iterator end() { return _data + _size; }

	/**
	 * @return const iterator pointing to first element for const vec.
	 */
	const_iterator begin() const { return _data; }

	/**
 * @return const iterator pointing after the last element for const vec.
 */
	const_iterator end() const { return _data + _size; }

	/**
	 * @return const iterator pointing to first element.
	 */
	const_iterator cbegin() const { return _data; }

	/**
	 * @return const iterator pointing after last element.
	 */
	const_iterator cend() const  { return _data + _size; }

};

template<class T, std::size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::at(std::size_t index)
{
	if (index >= _size)
	{
		throw std::out_of_range(OUT_OF_RANGE_ERR_MSG);
	}
	return _data[index];
}

template<class T, std::size_t StaticCapacity>
const T &VLVector<T, StaticCapacity>::at(std::size_t index) const
{
	if (index >= _size)
	{
		throw std::out_of_range(OUT_OF_RANGE_ERR_MSG);
	}
	return _data[index];
}

template<class T, std::size_t StaticCapacity>
bool VLVector<T, StaticCapacity>::operator==(const VLVector &other) const
{
	return end() - begin() == other.end() - other.begin()
			&& std::equal(begin(), end(), other.begin());
}

template<class T, std::size_t StaticCapacity>
std::size_t VLVector<T, StaticCapacity>::_cap(std::size_t additionalSize)
{
	return _size + additionalSize <= StaticCapacity?
	StaticCapacity: std::floor(GROWTH_FACTOR * (_size + additionalSize));

}

template<class T, std::size_t StaticCapacity>
template<class InputIterator>
typename VLVector<T, StaticCapacity>::iterator
VLVector<T, StaticCapacity>::insert(VLVector::const_iterator pos, InputIterator first,
									InputIterator last)
{
	std::size_t numOfElements = std::distance(first, last);
	std::size_t posIdx = pos - begin();
	auto pushFunc = [&]() {std::copy(first, last, _data + posIdx); };
	// Push the elements by using pushFunc after rest will be shifted right.
	_pushAt(pos, numOfElements, pushFunc);
	return begin() + posIdx; // Iterator to the first inserted element.
}

template<class T, std::size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator
VLVector<T, StaticCapacity>::erase(VLVector::const_iterator first, VLVector::const_iterator last)
{
	T* oldMem = nullptr;
	const_iterator initEnd = cend();
	std::size_t toRemoveSize = last - first, firstIdx = first - begin();
	if (_capacity > StaticCapacity && _size - toRemoveSize <= StaticCapacity)
	{
		std::copy(cbegin(), first, _staticMem);
		oldMem = _data;
		_data = _staticMem;
		_capacity = StaticCapacity;
	}
	std::copy(last, initEnd, begin() + firstIdx);
	_size -= toRemoveSize;
	delete[] oldMem;
	return begin() + firstIdx;
}

template<class T, std::size_t StaticCapacity>
void VLVector<T, StaticCapacity>::clear()
{
	erase(begin(), end());
}

template<class T, std::size_t StaticCapacity>
VLVector<T, StaticCapacity>& VLVector<T, StaticCapacity>::operator=(const VLVector &other)
{
	if (&other == this)
	{
		return *this;
	}
	if (_capacity > StaticCapacity)
	{
		delete[] _data;
	}
	_copyMembers(other);
	return *this;
}

template<class T, std::size_t StaticCapacity>
void VLVector<T, StaticCapacity>::_copyMembers(const VLVector &other)
{
	if (other._capacity > StaticCapacity)
	{
		_data = new T[other._capacity];
	}
	_capacity = other._capacity;
	std::copy(other.begin(), other.end(), begin());
	_size = other._size;
}

template<class T, std::size_t StaticCapacity>
template<class PushFunc>
void VLVector<T, StaticCapacity>::_pushAt(const T *pos, std::size_t numOfElements, PushFunc
										  pushElements)
{
	T* oldMem = nullptr;
	const_iterator initialEnd = end();
	if (_size + numOfElements > _capacity) // Reallocate if needed and copy [begin, first).
	{
		T* newMem = new T[_cap(numOfElements)];
		oldMem = _capacity <= StaticCapacity? nullptr: _data;
		_capacity = _cap(numOfElements);
		std::copy(cbegin(), pos, newMem);
		_data = newMem;
	}
	std::copy_backward(pos, initialEnd, end() + numOfElements); // Move elements by numOfElements.
	pushElements();
	_size += numOfElements;
	delete[] oldMem;

}

template<class T, std::size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator
        VLVector<T, StaticCapacity>::insert(VLVector::const_iterator pos, const T &value)
{
	std::size_t idx = pos - begin();
	_pushAt(pos, 1, [&]() {_data[idx] = value; });
	return begin() + idx;
}

template<class T, std::size_t StaticCapacity>
VLVector<T, StaticCapacity>::~VLVector()
{
	if (_capacity > StaticCapacity)
	{
		delete[] _data;
	}
}

#endif // VLVECTOR_HPP
