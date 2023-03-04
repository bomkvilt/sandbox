// https://leetcode.com/problems/snapshot-array

class SnapshotArray {
public:
    using TVersion = int;
    using TValue = int;
    using TIndex = int;

private:
    using TSnapshotsMap = std::map<TVersion, TValue>;
    using TVersionedData = std::unordered_map<TIndex, TSnapshotsMap>;

public:
    SnapshotArray(int length) {
        // void
    }

    void set(TIndex index, TValue val) {
        VersionedData[index][CurrentVersion] = val;
    }

    TVersion snap() {
        return CurrentVersion++;
    }

    TValue get(int index, TVersion snapID) {
        constexpr TValue DEFAULT = 0;

        auto& versions = VersionedData[index];
        auto iter = versions.upper_bound(snapID);
        return iter != versions.begin() ? std::prev(iter)->second : DEFAULT;
    }

private:    
    TVersionedData VersionedData;
    TVersion CurrentVersion = 0;
};
