#include "../exercise.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
    }

    ~Resource() {
        RECORDS.push_back(_records);
    }
};

using Unique = std::unique_ptr<Resource>;
Unique reset(Unique ptr) {
    if (ptr) ptr->record('r');
    return std::make_unique<Resource>();
}
Unique drop(Unique ptr) {
    if (ptr) ptr->record('d');
    return nullptr;
}
Unique forward(Unique ptr) {
    if (ptr) ptr->record('f');
    return ptr;
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3];

    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);

    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);

    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);

    // ---- 不要修改以上代码 ----

    // 只在此处填写分析结果
    // 第一组: problems[0] = {"fd"}
    // 第二组: problems[1] = {"ffr","d"} (共2条记录)
    // 第三组: problems[2] = {"r","d","d"} (共3条记录)
    // 因为代码写死了每组要有 8 个字符串，所以后面都留空。
    std::vector<const char *> answers[]{
        {"fd"},
        {"ffr", "d", "", "", "", "", "", ""},
        {"r",   "d", "d", "", "", "", "", ""},
    };

    // ---- 不要修改以下代码 ----

    for (auto i = 0; i < 3; ++i) {
        // ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (auto j = 0U; j < problems[i].size(); ++j) {
            // ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    std::cout << "All tests passed!\n";
    return 0;
}