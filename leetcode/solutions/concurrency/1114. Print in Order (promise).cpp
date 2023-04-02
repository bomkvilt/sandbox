// https://leetcode.com/problems/print-in-order/description/


class Foo {
private:
    std::promise<void> P1;
    std::promise<void> P2;

public:
    void first(function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();

        P1.set_value();
    }

    void second(function<void()> printSecond) {
        P1.get_future().wait();

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();

        P2.set_value();
    }

    void third(function<void()> printThird) {
        P2.get_future().wait();

        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};
