#pragma once
#include "MemorySearch.h"
#include <vector>
#include <string>
#include <functional>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <unordered_map>

// 高级内存搜索管理器
class AdvancedMemorySearcher {
public:
    // 搜索配置
    struct SearchConfig {
        int maxRetries = 3;                    // 最大重试次数
        int retryDelayMs = 100;                // 重试延迟(毫秒)
        int maxThreads = 0;                    // 最大线程数，0表示自动检测
        bool enableFallbackSearch = true;      // 启用备用搜索算法
        bool enablePatternValidation = true;   // 启用特征码验证
        
        SearchConfig() {
            // 自动检测硬件支持的线程数
            if (maxThreads == 0) {
                maxThreads = std::thread::hardware_concurrency();
                if (maxThreads == 0) maxThreads = 4; // 默认4个线程
            }
        }
    };
    
    // 搜索结果详情
    struct SearchResultDetail {
        std::string patternName;               // 特征码名称
        DWORD_PTR address = 0;                 // 找到的地址
        bool success = false;                  // 是否成功
        int retryCount = 0;                    // 重试次数
        double searchTimeMs = 0;               // 搜索耗时(毫秒)
        std::string errorMessage;              // 错误信息
        std::vector<DWORD_PTR> candidateAddresses; // 候选地址列表
        
        SearchResultDetail() {}
        SearchResultDetail(const std::string& name) : patternName(name) {}
    };
    
    // 搜索进度回调
    using ProgressCallback = std::function<void(const std::string& patternName, int progress, const std::string& status)>;
    
    // 完成回调
    using CompletionCallback = std::function<void(const std::vector<SearchResultDetail>& results)>;

private:
    std::vector<SearchPattern> patterns_;      // 特征码列表
    SearchConfig config_;                      // 搜索配置
    std::atomic<bool> isSearching_{false};     // 是否正在搜索
    std::atomic<int> completedPatterns_{0};    // 已完成的特征码数量
    std::mutex resultsMutex_;                  // 结果互斥锁
    std::vector<SearchResultDetail> results_;  // 搜索结果
    
    // 进度和完成回调
    ProgressCallback progressCallback_;
    CompletionCallback completionCallback_;

public:
    AdvancedMemorySearcher() = default;
    ~AdvancedMemorySearcher();
    
    // 设置搜索配置
    void setConfig(const SearchConfig& config);
    
    // 添加特征码
    void addPattern(const SearchPattern& pattern);
    void addPatterns(const std::vector<SearchPattern>& patterns);
    
    // 清空特征码
    void clearPatterns();
    
    // 设置回调函数
    void setProgressCallback(ProgressCallback callback);
    void setCompletionCallback(CompletionCallback callback);
    
    // 开始搜索（异步）
    bool startSearchAsync(const std::string& processName = "MuMuVMMHeadless.exe");
    
    // 停止搜索
    void stopSearch();
    
    // 同步搜索
    std::vector<SearchResultDetail> searchSync(const std::string& processName = "MuMuVMMHeadless.exe");
    
    // 检查是否正在搜索
    bool isSearching() const { return isSearching_; }
    
    // 获取搜索结果
    std::vector<SearchResultDetail> getResults() const;
    
    // 获取特定特征码的结果
    SearchResultDetail getResult(const std::string& patternName) const;
    
    // 验证特征码地址
    bool validateAddress(const std::string& patternName, DWORD_PTR address);
    
    // 批量验证地址
    std::vector<std::string> validateAllAddresses();

private:
    // 搜索线程函数
    void searchThread(const std::string& processName);
    
    // 单个特征码搜索
    SearchResultDetail searchSinglePattern(const SearchPattern& pattern, HANDLE hProcess);
    
    // 备用搜索算法
    SearchResultDetail fallbackSearch(const SearchPattern& pattern, HANDLE hProcess);
    
    // 更新进度
    void updateProgress(const std::string& patternName, int progress, const std::string& status);
    
    // 处理搜索结果
    void handleSearchResult(const SearchResultDetail& result);
    
    // 验证内存地址
    bool isValidMemoryAddress(HANDLE hProcess, DWORD_PTR address);
    
    // 读取内存验证
    bool readAndValidateMemory(HANDLE hProcess, DWORD_PTR address, const std::vector<BYTE>& expectedBytes);
};

// 全局高级搜索管理器实例
AdvancedMemorySearcher& getAdvancedSearcher();

// 便利函数：使用高级搜索器搜索特征码
void advancedSearchPatterns(const std::vector<SearchPattern>& patterns,
                          AdvancedMemorySearcher::CompletionCallback callback,
                          const std::string& processName = "MuMuVMMHeadless.exe");

// 便利函数：带进度显示的高级搜索
void advancedSearchWithProgress(const std::vector<SearchPattern>& patterns,
                              AdvancedMemorySearcher::ProgressCallback progressCallback,
                              AdvancedMemorySearcher::CompletionCallback completionCallback,
                              const std::string& processName = "MuMuVMMHeadless.exe");