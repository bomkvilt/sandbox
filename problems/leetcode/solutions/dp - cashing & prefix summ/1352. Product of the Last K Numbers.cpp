// https://leetcode.com/problems/product-of-the-last-k-numbers/


class ProductOfNumbers {
public:
    ProductOfNumbers() {
        Products_.push_back(1);
    }
    
    void add(int num) {
        if (num) {
            Products_.push_back(num * Products_.back());
        }
        else {
            Products_.push_back(1);
            Zeros_.push_back(Products_.size() - 1);
        }
    }

    int getProduct(int k) {
        const auto head = Products_.size() - k;
        if (Zeros_.size() && Zeros_.back() >= head) {
            return 0;
        }
        return Products_.back() / Products_[head - 1];
    }
    
private:
    std::vector<int64_t> Products_;
    std::vector<size_t> Zeros_;
};
