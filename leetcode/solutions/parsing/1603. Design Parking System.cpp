// https://leetcode.com/problems/design-parking-system/

class ParkingSystem {
public:
    ParkingSystem(int big, int medium, int small) {
        Vacancies[0] = big;
        Vacancies[1] = medium;
        Vacancies[2] = small;
    }
    
    bool addCar(int carType) {
        int idx = carType - 1;
        if (Vacancies[idx] <= 0) {
            return false;
        }
        Vacancies[idx]--;
        return true;
    }
    
private:
    std::array<int, 3> Vacancies;
};
