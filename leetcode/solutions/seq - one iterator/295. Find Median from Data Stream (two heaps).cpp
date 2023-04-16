// https://leetcode.com/problems/find-median-from-data-stream/

#include <queue>
#include <unordered_map>
#include <vector>


class MedianFinder {
private:
    using TElement = int;
    using TMaxHeap = std::priority_queue<TElement, std::vector<TElement>, std::less<TElement>>;
    using TMinHeap = std::priority_queue<TElement, std::vector<TElement>, std::greater<TElement>>;

    TMinHeap MinHeap;
    TMaxHeap MaxHeap;

public:
    void addNum(int num) {
        if (MinHeap.size() && MinHeap.top() < num) {
            AddElement(MinHeap, num);
        }
        else {
            AddElement(MaxHeap, num);
        }
        Ballance();
    }

    double findMedian() {
        if (const auto k = MinHeap.size() + MaxHeap.size(); k & 1) {
            return MaxHeap.top();
        }
        const double a = MaxHeap.top();
        const double b = MinHeap.top();
        return (a + b) / 2;
    }

private:
    template<typename THeap>
    void AddElement(THeap& heap, const TElement& target) {
        heap.push(target);
    }

    void Ballance() {
        while (MinHeap.size() > MaxHeap.size()) {
            MoveTop(MinHeap, MaxHeap);
        }
        while (MaxHeap.size() - MinHeap.size() > 1) {
            MoveTop(MaxHeap, MinHeap);
        }
    }

    template<typename THeapA, typename THeapB>
    void MoveTop(THeapA& src, THeapB& dst) {
        dst.push(src.top());
        src.pop();
    }
};
