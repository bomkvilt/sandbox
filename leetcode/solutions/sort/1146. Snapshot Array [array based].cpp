// https://leetcode.com/problems/snapshot-array

class SnapshotArray {
public:
    using TVersion = int;
    using TValue = int;

private:
    struct TSnapshot {
        TVersion Version = 0;
        TValue Value = 0;
    };
    
    using TSnapshotsList = std::vector<TSnapshot>;
    using TVersionedData = std::vector<std::unique_ptr<TSnapshotsList>>;

public:
    SnapshotArray(int length) {
        VersionedData = TVersionedData(length);
    }

    void set(int index, int val) {
        auto& vptr = VersionedData[index];
        if (!vptr) {
            vptr.reset(new TSnapshotsList());
        }

        if (vptr->size()) {
            auto& back = vptr->back();
            if (back.Version == CurrentVersion) {
                back.Value = val;
                return;
            }
        }

        vptr->push_back(TSnapshot{
            .Version = CurrentVersion,
            .Value = val,
        });
    }

    TVersion snap() {
        return CurrentVersion++;
    }

    TValue get(int index, TVersion snapID) {
        constexpr TValue DEFAULT = 0;

        auto& vptr = VersionedData[index];
        if (!vptr) {
            return DEFAULT;
        }

        auto itl = vptr->rbegin();
        auto itr = vptr->rend();
        auto iter = std::lower_bound(itl, itr, snapID,
            [](const TSnapshot& snaphot, TVersion target) {
                return snaphot.Version > target;
            }
        );
        return (iter != itr) ? (iter)->Value : DEFAULT;
    }

private:    
    TVersionedData VersionedData;
    TVersion CurrentVersion = 0;
};
