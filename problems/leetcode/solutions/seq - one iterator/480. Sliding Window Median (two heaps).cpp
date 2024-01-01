// https://leetcode.com/problems/sliding-window-median/

#include <queue>
#include <unordered_map>
#include <vector>


template<typename TElement>
struct MinMaxHeap {
public:
    using TMaxHeap = std::priority_queue<TElement, std::vector<TElement>, std::less<TElement>>;
    using TMinHeap = std::priority_queue<TElement, std::vector<TElement>, std::greater<TElement>>;
    using TTombStones = std::unordered_map<TElement, int>;

    // [ X, X, M, X, X     ]: partition_point
    // [max heap] [min heap]
    TMinHeap MinHeap;
    TMaxHeap MaxHeap;
    TTombStones Tombstones;

    int MinHeapPower = 0;
    int MaxHeapPower = 0;

public:
    void AddElement(TElement target) {
        if (MinHeapPower > 0 && MinHeap.top() < target) {
            MinHeap.push(target);
            MinHeapPower += 1;
        }
        else {
            MaxHeap.push(target);
            MaxHeapPower += 1;
        }
    }

    void Ballance() {
        while (MinHeapPower > MaxHeapPower) {
            MoveTop(MinHeap, MinHeapPower, MaxHeap, MaxHeapPower);
        }
        while (MaxHeapPower - MinHeapPower > 1) {
            MoveTop(MaxHeap, MaxHeapPower, MinHeap, MinHeapPower);
        }
        DropTombStones(MinHeap);
        DropTombStones(MaxHeap);
    }

    void DropElement(const TElement& target) {
        if (MinHeapPower > 0 && MinHeap.top() <= target) {
            DropElement(MinHeap, MinHeapPower, target);
        }
        else if (MaxHeapPower > 0) {
            DropElement(MaxHeap, MaxHeapPower, target);
        }
        Ballance();
    }

private:
    template<typename THeapA, typename THeapB>
    static void MoveTop(THeapA& src, int& srcPow, THeapB& dst, int& dstPow) {
        dst.push(src.top());
        src.pop();
        srcPow -= 1;
        dstPow += 1;
    }

    template<typename THeap>
    void DropElement(THeap& heap, int& heapPow, const TElement& target) {
        heapPow -= 1;
        if (heap.top() == target) {
            heap.pop();
        }
        else {
            Tombstones[target] += 1;
        }
    }

    template<typename THeap>
    void DropTombStones(THeap& heap) {
        while (heap.size()) {
            auto pos = Tombstones.find(heap.top());
            if (pos == Tombstones.end()) {
                break;
            }

            heap.pop();
            if ((--pos->second) <= 0) {
                Tombstones.erase(pos);
            }
        }
    }
};


class Solution {
public:
    std::vector<double> medianSlidingWindow(std::vector<int>& nums, const int k) {
        const auto n = nums.size();
        if (k <= 0 || n <= 0) {
            return {};
        }

        MinMaxHeap<int> heap;

        // create window
        for (size_t i = 0; i < k; ++i) {
            heap.AddElement(nums[i]);
        }
        heap.Ballance();

        // iterate
        std::vector<double> medians;
        for (size_t i = k; i < n; ++i) {
            medians.push_back(CalcMedian(heap, k));

            const auto oldElement = nums[i - k];
            const auto newElement = nums[i - 0];
            heap.DropElement(oldElement);
            heap.AddElement(newElement);
            heap.Ballance();
        }

        medians.push_back(CalcMedian(heap, k));
        return std::move(medians);
    }

private:
    double CalcMedian(const MinMaxHeap<int>& heap, const int k) {
        if (k & 1) {
            return heap.MaxHeap.top();
        }
        const double a = heap.MaxHeap.top();
        const double b = heap.MinHeap.top();
        return (a + b) / 2;
    }
};
