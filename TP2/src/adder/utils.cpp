#include <vector>

using namespace std;

vector<vector<char>> splitVectorIntoChunks(const vector<char> &source, size_t chunkSize)
{
    vector<vector<char>> result;
    result.reserve((source.size() + chunkSize - 1) / chunkSize);

    auto start = source.begin();
    auto end = source.end();

    while (start != end)
    {
        auto next = distance(start, end) >= chunkSize
                        ? start + chunkSize
                        : end;

        result.emplace_back(start, next);
        start = next;
    }

    return result;
}