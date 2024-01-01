// https://leetcode.com/problems/print-in-order/description/

class Foo {
private:
    std::mutex Mutex;
    std::condition_variable CV;
    std::atomic<int> Stage = 0;

public:
    void first(function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        
        Stage = 1;
        CV.notify_all();
    }

    void second(function<void()> printSecond) {
        while (Stage < 1) {
            std::unique_lock lk(Mutex);
            CV.wait(lk);
        }

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();

        Stage = 2;
        CV.notify_one();
    }

    void third(function<void()> printThird) {
        while (Stage < 2) {
            std::unique_lock lk(Mutex);
            CV.wait(lk);
        }

        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};
