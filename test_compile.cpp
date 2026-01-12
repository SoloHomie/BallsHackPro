#include "MemorySearch.h"
#include <iostream>
#include <windows.h>

int main() {
    std::cout << "Testing MemorySearch compilation..." << std::endl;
    
    // 创建SundaySearcher对象
    SundaySearcher searcher;
    
    // 测试添加特征码
    std::vector<BYTE> pattern = {0x01, 0x02, 0x03, 0x04};
    searcher.addPattern(pattern, "test_pattern", 1);
    
    std::cout << "MemorySearch test completed successfully!" << std::endl;
    return 0;
}