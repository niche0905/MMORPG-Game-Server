#pragma once

struct alignas(CACHE_LINE_SIZE) EpochSlot
{
	std::atomic_ullong slot;
};

template<class T>
struct alignas(CACHE_LINE_SIZE) FreeList
{
	std::queue<T*> free_queue;
};


//--------------------------------------------------------
// 
// NOTICE: T는 멤버로 uint64 _ebr_number를 꼭 지녀야 한다
//		   Reset(Args&&... args) 멤버 함수가 있어야 한다
//									(생성자 대용)
// 
//--------------------------------------------------------
template<class T>
class EpochBasedReclamation
{
	static uint64 MAX_ULLONG = std::numeric_limits<uint64>::max();

private:
	int32 _thread_num;

	std::atomic_ullong _epoch_counter;
	std::vector<EpochSlot> _epoch_array;
	std::vector<FreeList<T>> _free_list_array;

public:
	EpochBasedReclamation();
	EpochBasedReclamation(int32 thread_num);
	~EpochBasedReclamation();

	void Clear();

	void Reuse(T* ptr);
	template<typename... Args>
	T* Get(Args&&... args);
	
	void StartEpoch();
	void EndEpoch();

};


template<class T>
using EBR = EpochBasedReclamation;


template<class T>
inline EpochBasedReclamation<T>::EpochBasedReclamation()
	: EpochBasedReclamation(std::thread::hardware_concurrency())
{

}

template<class T>
inline EpochBasedReclamation<T>::EpochBasedReclamation(int32 thread_num)
	: _thread_num{ thread_num }
	, _epoch_counter{ 0 }
{
	_epoch_array.resize(_thread_num);
	_free_list_array.resize(_thread_num);
}

template<class T>
inline EpochBasedReclamation<T>::~EpochBasedReclamation()
{
	Clear();
}

template<class T>
inline void EpochBasedReclamation<T>::Clear()
{
	for (auto& free_list : _free_list_array) {
		while (not free_list.empty()) {
			auto ptr = free_list.front(); free_list.pop();
			delete ptr;
		}
	}

	_epoch_counter.store(0);
}

template<class T>
inline void EpochBasedReclamation<T>::Reuse(T* ptr)
{
	ptr->_ebr_number = _epoch_counter;
	_free_list_array[thread_id].free_queue.push(ptr);
}

template<class T>
template<typename... Args>
inline T* EpochBasedReclamation<T>::Get(Args&&... args)
{
	if (_free_list_array[thread_id].free_queue.empty()) {
		return new T{std::forward<Args>(args)...};
	}

	T* ptr = _free_list_array[thread_id].free_queue.front();
	for (int i = 0; i < _thread_num; ++i) {
		if (_epoch_array[i] < ptr->_ebr_number) {
			return new T{ std::forward<Args>(args)... };
		}
	}

	_free_list_array[thread_id].free_queue.pop();
	ptr->Reset(std::forward<Args>(args)...);
	return ptr;
}

template<class T>
inline void EpochBasedReclamation<T>::StartEpoch()
{
	uint64 epoch = ++_epoch_counter;
	_epoch_array[thread_id] = epoch;
}

template<class T>
inline void EpochBasedReclamation<T>::EndEpoch()
{
	_epoch_array[thread_id] = MAX_ULLONG;
}
