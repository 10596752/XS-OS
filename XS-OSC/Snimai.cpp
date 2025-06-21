//Code by: UTF-8
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <bcrypt.h> // For SHA-256 hashing
#include <shellapi.h> // For opening web browser
#include <mmsystem.h> // For playing audio
#include <chrono>
#include "src/header/yinhebrower.h"
#include "src/header/begin.h"
#include "src/header/jingziqi.h"
#include <direct.h>
#include <iomanip>
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "winmm.lib")

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

using namespace std;

// Global variables
string name;
string current_dir;
vector<string> menu_stack = {"main"};
int bz = 0;

// Function to hash password using SHA-256
static string hash_password(const string& password) {
    BCRYPT_ALG_HANDLE hAlg = NULL;
    BCRYPT_HASH_HANDLE hHash = NULL;
    NTSTATUS status = 0;
    DWORD cbData = 0, cbHash = 0, cbHashObject = 0;
    PBYTE pbHashObject = NULL;
    PBYTE pbHash = NULL;
    string hashed;

    // Open an algorithm handle
    status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0);
    if (!NT_SUCCESS(status)) {
        cerr << "Error opening algorithm provider" << endl;
        return "";
    }

    // Calculate the size of the buffer to hold the hash object
    status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);
    if (!NT_SUCCESS(status)) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error getting object length" << endl;
        return "";
    }

    // Allocate the hash object on the heap
    pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
    if (NULL == pbHashObject) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error allocating hash object" << endl;
        return "";
    }

    // Calculate the length of the hash
    status = BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);
    if (!NT_SUCCESS(status)) {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error getting hash length" << endl;
        return "";
    }

    // Allocate the hash buffer on the heap
    pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
    if (NULL == pbHash) {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error allocating hash buffer" << endl;
        return "";
    }

    // Create a hash
    status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, NULL, 0, 0);
    if (!NT_SUCCESS(status)) {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
        HeapFree(GetProcessHeap(), 0, pbHash);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error creating hash" << endl;
        return "";
    }

    // Hash the password
    status = BCryptHashData(hHash, (PBYTE)password.c_str(), (ULONG)password.length(), 0);
    if (!NT_SUCCESS(status)) {
        BCryptDestroyHash(hHash);
        HeapFree(GetProcessHeap(), 0, pbHashObject);
        HeapFree(GetProcessHeap(), 0, pbHash);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error hashing data" << endl;
        return "";
    }

    // Finalize the hash
    status = BCryptFinishHash(hHash, pbHash, cbHash, 0);
    if (!NT_SUCCESS(status)) {
        BCryptDestroyHash(hHash);
        HeapFree(GetProcessHeap(), 0, pbHashObject);
        HeapFree(GetProcessHeap(), 0, pbHash);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        cerr << "Error finalizing hash" << endl;
        return "";
    }

    // Convert the hash to a hex string
    for (DWORD i = 0; i < cbHash; i++) {
        char buf[3];
        sprintf_s(buf, "%02x", pbHash[i]);
        hashed += buf;
    }

    // Clean up
    BCryptDestroyHash(hHash);
    HeapFree(GetProcessHeap(), 0, pbHashObject);
    HeapFree(GetProcessHeap(), 0, pbHash);
    BCryptCloseAlgorithmProvider(hAlg, 0);

    return hashed;
}

// Function to clear console
static void clear_screen() {
    std::cout << "\033c";
}

// Function to print text with delay (simulating Python's newprint)
static void newprint(const string& text, int delay_ms = 50) {
    for (char a : text) {
        std::cout << a;
        std::cout.flush();
        Sleep(delay_ms);
    }
    std::cout << endl;
}

// Function to play sound
static void play_sound(const string& path) {
    // Convert std::string to std::wstring for PlaySoundW
    std::wstring wpath(path.begin(), path.end());
    PlaySoundW(wpath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

// Function to stop sound
static void stop_sound() {
    PlaySound(NULL, NULL, 0);
}

// Function to open web browser
static void open_browser(const string& url) {
    std::wstring wurl(url.begin(), url.end());
    ShellExecuteW(NULL, L"open", wurl.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// Function to get current time in HH:MM format
static string get_current_time() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char buffer[6];
    strftime(buffer, sizeof(buffer), "%H:%M", &ltm);
    return string(buffer);
}

// Function to get current hour
static int get_current_hour() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    return ltm.tm_hour;
}



// Main function
int main() {
    SetConsoleOutputCP(65001);    
    SetConsoleCP(CP_UTF8);
    // Initialize paths
    current_dir = "G:\\Dev\\XS-OSC\\src\\";
    string name_path = current_dir + "name.csv";
    string password_path = current_dir + "password.csv";
    string ji_path = current_dir + "sound\\系.mp3";
    string fontaine_path = current_dir + "sound\\枫丹.mp3";
    string chongzi_path = current_dir + "sound\\萤火虫.mp3";
    string Luminescence_path = current_dir + "sound\\Luminescence.mp3";
    string MARENOL_path = current_dir + "sound\\MARENOL.mp3";
    string aoharu_path = current_dir + "sound\\REAoharu.flac";
    string qingchun_path = current_dir + "sound\\青春档案.mp3";

    // Read name and password from files
    ifstream name_file(name_path);
    if (name_file.is_open()) {
        getline(name_file, name);
        name_file.close();
    }

    string stored_password;
    ifstream password_file(password_path);
    if (password_file.is_open()) {
        getline(password_file, stored_password);
        password_file.close();
    }
    clear_screen();
    Sleep(1500);
    begin();
    // System registration/login
    clear_screen();
    std::cout << "\033[032m         X S - O S系 统 欢 迎 您 ！\033[0m" << endl;
    std::cout << "\033[032m         Byteburst Studio©\033[0m" << endl;
    std::cout << "注意：每台电脑只能注册一个账号，如果重新注册将覆盖原来的账号！" << endl;
    std::cout << "请输入你的密码(按0注册)：";
    string a;
    getline(cin, a);

    if (a == "0") {
        clear_screen();
        std::cout << "请输入你的用户名：";
        getline(cin, name);
        
        ofstream name_out(name_path);
        if (name_out.is_open()) {
            name_out << name;
            name_out.close();
        }
        
        clear_screen();
        while (true) {
            std::cout << "请输入你的用户密码(不可以是0！)：";
            string password;
            getline(cin, password);
            
            
            string hashed_password = hash_password(password);
            clear_screen();
            
            std::cout << "请再次输入你的用户密码确认：";
            string confirm_password;
            getline(cin, confirm_password);
            string hashed_confirm_password = hash_password(confirm_password);
            
            if (hashed_password == hashed_confirm_password) {
                clear_screen();
                std::cout << "\033[32m 注册成功！\033[0m" << endl;
                Sleep(2000);
                clear_screen();
                
            ofstream password_out(password_path);
            if (password_out.is_open()) {
                password_out << hashed_password;
                password_out.close();
            }
                std::cout << "请输入你的密码：";
                string password;
                getline(cin, password);
                string hashed_input_password = hash_password(password);
                
                if (hashed_input_password == hashed_password) {
                    clear_screen();
                    std::cout << "\033[32m 登录成功！ \033[0m" << endl;
                    Sleep(2000);
                    clear_screen();
                } else {
                    std::cout << "密码错误，请重新输入密码。" << endl;
                    Sleep(500);
                    clear_screen();
                    continue;
                }
                break;
            } else {
                std::cout << "两次输入的密码不一致，请重新输入密码。" << endl;
            }
        }
    } else if (a == "114514") {
        clear_screen();
        std::cout << "恭喜你发现了彩蛋！接下来我会播放作者最喜欢的曲子！" << endl;
        Sleep(2000);
        play_sound(fontaine_path);
        std::cout << "1.停止播放";
        string rainbow;
        getline(cin, rainbow);
        if (rainbow == "1") {
            stop_sound();
            clear_screen();
            return 0;
        }
    } else {
        string a_hashed_password = hash_password(a);
        if (a_hashed_password == stored_password) {
            clear_screen();
            std::cout << "\033[32m 登录成功！ \033[0m" << endl;
            Sleep(1000);
            clear_screen();
        } else {
            clear_screen();
            std::cout << "密码错误。" << endl;
            Sleep(1000);
            return 0;
        }
    }

    if (name == "SigN_285") {
        bz = 1;
    }
    // Welcome message
    while (true) {
    int clock = get_current_hour();
    if (clock < 24 && clock >= 18) {
    std::cout << "\033[32m 晚上好，注意休息哦, " << name << "!\033[0m" << endl;
    } else if (clock < 18 && clock >= 13) {
    std::cout << "\033[32m 下午好, " << name << "!\033[0m" << endl;
    } else if (clock < 13 && clock >= 12) {
    std::cout << "\033[32m 中午好, " << name << "!\033[0m" << endl;
    } else if (clock >= 7) {
    std::cout << "\033[32m 上午好, " << name << "!\033[0m" << endl;
    } else {
    std::cout << "\033[32m 凌晨不好，赶紧睡觉, " << name << "!\033[0m" << endl;
    }

    // Main menu loop
    while (true) {
        string current_menu = menu_stack.back();
        
        if (current_menu == "main") {
            std::cout << "版本：4.0.0" << endl;
            string time = get_current_time();
            std::cout << "现在时间：" << time << endl;
            std::cout << "1.开始" << endl;
            std::cout << "2.文件资源管理器" << endl;
            std::cout << "3.应用" << endl;
            std::cout << "4.意见反馈" << endl;
            std::cout << "5.停止音乐" << endl;
            
            string windows_answer;
            getline(cin, windows_answer);
            
            if (windows_answer == "1" || windows_answer == "2" || windows_answer == "3" || 
                windows_answer == "4" || windows_answer == "5") {
                menu_stack.push_back(windows_answer);
            } else {
                std::cout << "输入无效，请重新输入，选择1、2、3、4或5。" << endl;
                Sleep(2000);
                clear_screen();
                continue;
            }
        }
        // Start menu
        else if (current_menu == "1") {
            clear_screen();
            std::cout << "1.关机" << endl;
            std::cout << "2.系统自检" << endl;
            std::cout << "3.返回" << endl;
            
            string kaishi;
            getline(cin, kaishi);
            
            if (kaishi == "1") {
                clear_screen();
                newprint("感谢您的使用，再见！");
                return 0;
            } else if (kaishi == "2") {
                clear_screen();
                std::cout << "开始检查...";
                std::cout << "\n\n软件检查" << endl;
                Sleep(1000);
                std::cout << "\033[037mCPU 运算逻辑部件                     \033[032m 正确\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 浮点算术运算                     \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 地址运算                         \033[032m 正常\033[037m" << endl;
                Sleep(500);
                
                std::cout << "CPU 专用寄存器                       \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 控制寄存器                       \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 高速缓冲存储器                   \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 运算码                           \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU ALU电路                          \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 时钟频率      2400兆赫  （MHz）  \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 脉冲信号       4.8千兆赫（GHz）  \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU PowerPC处理器                    \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU SPARC处理器                      \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU PA-RISC处理器                    \033[032m 正常\033[037m" << endl; 
                Sleep(500);
                std::cout << "CPU MIPS处理器                       \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU Alpha处理器                      \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU x86-to-IA-64的解码器             \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 单芯片多处理器                   \033[032m 正常\033[037m\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 多核处理器                       \033[032m 正常\033[037m\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU Windows服务器                       \033[032m 正常\033[037m\033[037m" << endl; 
                Sleep(500);
                std::cout << "CPU 加法器                           \033[032m 正常\033[037m\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 寄存器                           \033[032m 正常\033[037m\033[037m" << endl;
                Sleep(500);

                std::cout << "CPU 堆栈存储器                       \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "CPU 主板硅处理器                     \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "BUS 系统总线传输                     \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "单通道接收 1420兆赫                  \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "双通道接收 2800兆赫                  \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "单向输出口 4.6KB                     \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "双向输出口 5.4KB                     \033[032m 正常\033[037m" << endl;
                Sleep(500);
                std::cout << "状态寄存器                           \033[032m 正常\033[000m" << endl;

                Sleep(1000);

                std::cout << "\033[037m      " << endl;
                std::cout << "\033[037m      " << endl;
                std::cout << "\033[037m测试启动状况" << endl;Sleep(1000);
                std::cout << "\033[037m      " << endl;
                Sleep(500);
                std::cout << "\033[031;036m检查软件" << endl;
                Sleep(500);
                std::cout << "\033[036m引导程序启动" << endl;
                Sleep(500);
                std::cout << "\033[036m操作系统加载" << endl;
                Sleep(500);
                std::cout << "\033[036m启动软件" << endl;
                Sleep(500);
                std::cout << "\033[036m启动计算主控" << endl;
                Sleep(500);
                std::cout << "\033[036m加载差分模块" << endl;
                Sleep(500);
                std::cout << "\033[036m加载有限元模块" << endl;
                Sleep(500);
                std::cout << "\033[036m加载谱方法模块" << endl;
                Sleep(500);
                std::cout << "\033[036m调入初始条件参数" << endl;
                Sleep(500);
                std::cout<<endl;
                std::cout << "检查完成!";
                Sleep(1000);
                clear_screen();
                menu_stack.pop_back();
                continue;
            } else if (kaishi == "3") {
                clear_screen();
                menu_stack.pop_back();
                continue;
            } else {
                std::cout << "输入无效，请重新输入，选择1、2或输入“返回”。" << endl;
                continue;
            }
        }
        // File explorer menu
        else if (current_menu == "2") {
            clear_screen();
            std::cout << "1.C:\\ 系统文件 ■□□□□□□□□已用20GB，总空间1TB" << endl;
            std::cout << "2.D:\\ ■■□□□□□□□ 已用500MB,总空间50GB" << endl;
            std::cout << "3.返回" << endl;
            
            string explorer;
            getline(cin, explorer);
            
            if (explorer == "1") {
                clear_screen();
                if (bz == 1) {
                    std::cout << "你竟然发现了彩蛋！（有两个）那么，就让我恶搞一下吧！" << endl;
                    std::system("shutdown -s -t 30 -c 你的电脑将会在30秒后关机，请保存好你的工作程序。");
                    menu_stack.pop_back();
                } else {
                    std::cout << "无管理员权限。" << endl;
                    Sleep(1000);
                    clear_screen();
                    menu_stack.pop_back();
                }
            } else if (explorer == "2") {
                clear_screen();
                std::cout << "1.工具" << endl;
                std::cout << "2.小说" << endl;
                std::cout << "3.返回" << endl;
                
                string d_explorer;
                getline(cin, d_explorer);
                
                if (d_explorer == "1") {
                    clear_screen();
                    std::cout << "1.系统版权" << endl;
                    std::cout << "2.系统版本" << endl;
                    std::cout << "3.V4.0.0更新日志" << endl;
                    std::cout << "4.返回" << endl;
                    
                    string d_tools;
                    getline(cin, d_tools);
                    
                    if (d_tools == "1") {
                        clear_screen();
                        std::cout << "此系统为SigN_285本人编写，任何人不得抄袭，解释权归ByteBurst技术盛宴工作室所有。" << endl;
                        std::cout << "——————————————————————————————————————————————————" << endl;
                        std::cout << "Byteburst技术盛宴工作室©️" << endl;
                        Sleep(3000);
                        clear_screen();
                        menu_stack.pop_back();
                        continue;
                    } else if (d_tools == "2") {
                        clear_screen();
                        std::cout << "系统名称：XS-OSpy" << endl;
                        std::cout << "版本号：4.0.0" << endl;
                        std::cout << "内部版本号：22635.3:7000" << endl;
                        std::cout << "作者联系方式：" << endl;
                        std::cout << "    QQ:2838679223" << endl;
                        std::cout << "    微信号：SigN_285fu" << endl;
                        Sleep(15000);
                        clear_screen();
                        menu_stack.pop_back();
                        continue;
                    } else if (d_tools == "3") {
                        clear_screen();
                        std::cout << "V4.0.0更新日志：" << endl;
                        std::cout << "新增音乐：RE Aoharu - Nor" << endl;
                        std::cout << "首次将Python改为C++" << endl;
                        std::cout << "修复已知bug" << endl;
                        Sleep(5000);
                        clear_screen();
                        menu_stack.pop_back();
                        continue;
                    } else if (d_tools == "4") {
                        clear_screen();
                        menu_stack.pop_back();
                        continue;
                    } else {
                        std::cout << "输入无效，请重新输入，选择1、2或3。" << endl;
                        continue;
                    }
                } else if (d_explorer == "2") {
                    clear_screen();
                    std::cout << "1.MARENOL SigN_285&(特邀嘉宾)LeaF" << endl;
                    std::cout << "2.石林  SigN_285" << endl;
                    std::cout << "3.返回" << endl;
                    
                    string xiaoshuo;
                    getline(cin, xiaoshuo);
                    
                    if (xiaoshuo == "1") {
                        clear_screen();
                        newprint(R"(
在2018年，有一个女孩，名叫美穗，从二年级开始就患上了重度抑郁症。他去找心理医生开了药，可是没有任何作用。她停止了服药，寻找自杀的方法。
一天，    她在手机上看到一个名字叫做“MARENOL”的抗抑郁药品，治疗效果很强，但是副作用是：
“在真实无比的噩梦中死亡。”
她觉得这种药以治疗为主，于是刚买回家，她就吞了好几片。
她低估了这个药的副作用。
晚上入梦后，美穗发现自己正躺在铁轨上。她刚想站起来，“唰”，一辆高速飞驰的火车直接从美穗的升上碾过去，把她轧死了。（1.被轧死）
她到了另一个地方。只不过，这一次她在自由落体。周边全部是触手。过了一小会儿，触手上长出了蓝色的刀，向她刺过去，美穗直接在自由落体中被刺杀了。（2.被刺杀）
她到了一个工厂，但是她被钉在了一块铁板上，周边全部都是齿轮。她试图挣脱，但是没有任何用处。她在那里等待救援。突然，齿轮开始转动，从美穗的身上轧过去，她被高速转动的齿轮分尸了。（3.被齿轮分尸）
美穗这一次穿越到了一片海洋中，但是由于水里没有任何氧气，没多久她就窒息淹死了。（4.淹死）
美穗到了一片花海，开着五颜六色的花朵，有黄的蓝的紫的红的……她往花海中走去，突然没了知觉。原来，她彼岸花中毒了。（5.彼岸花中毒）
美穗出现在了一片黑暗中。突然！一片蓝色的眼睛睁开，闭上。又睁开，又闭上。美穗在这个“蓝色恐怖”的世界中惊吓死亡了。（6.注视恐惧症）
美穗刚刚穿越，就被绳子绑住了脖子，险些被吊死。绳子断裂后，她以为获得了新生，可是，黑暗下面是一根尖刺，她被这根尖刺穿过了身体，死了。（7.被尖刺穿过身体死亡）
美穗又到了一片黑暗。她以为要醒来了，可是她的手快速的长出了霉菌。她在黑暗中还在疑惑呢，只见天空中落下来了一根又细又长的黑针，把她的脑浆都炸出来了。（8.脑浆爆裂）
这之后，她没有梦到更多，她在现实世界中大脑休克死亡了。
其实，副作用后面还有一句：
        “少数人服用后会在梦中大脑休克死亡。”
美穗死后，她的灵魂飘到了一片蓝天白云的花海。这不是以前梦中的彼岸花，而是开着笑脸的小花（fa），从花海中穿出了一个戴着帽子的蒙面人。“你是来送我走的吧。”美穗问。“是的。”他回答道，并拿出了一个盛着少量水的瓶子。在那一瞬间，美穗变成了一片桃花瓣，倒吸入了瓶子，水被染红了，里面盛着美穗的灵魂。随后，蒙面人就把美穗带到了天堂。
Tip：不要喝奇怪的品红色药水！
)");
                        std::cout << "按0退出";
                        string xiaoshuo_shiling_answer;
                        getline(cin, xiaoshuo_shiling_answer);
                        if (xiaoshuo_shiling_answer == "0") {
                            clear_screen();
                            menu_stack.pop_back();
                            continue;
                        } else {
                            std::cout << "输入无效，请重新输入，选择0。" << endl;
                            continue;
                        }
                    } else if (xiaoshuo == "3") {
                        clear_screen();
                        menu_stack.pop_back();
                        continue;
                    } else {
                        std::cout << "输入无效，请重新输入，选择1或3。" << endl;
                        continue;
                    }
                } else if (d_explorer == "3") {
                    clear_screen();
                    menu_stack.pop_back();
                    continue;
                } else {
                    std::cout << "输入无效，请重新输入，选择1、2或3。" << endl;
                    continue;
                }
            } else if (explorer == "3") {
                clear_screen();
                menu_stack.pop_back();
                continue;
            } else {
                std::cout << "输入无效，请重新输入，选择1、2或3。" << endl;
                continue;
            }
        }
        // Applications menu
        else if (current_menu == "3") {
            clear_screen();
            std::cout << "1.井字棋（人机）" << endl;
            std::cout << "2.银河浏览器（转自曹艺登YS-OS）" << endl;
            std::cout << "3.音乐（持续更新，每更一个版本加一首）" << endl;
            std::cout << "4.返回" << endl;
            
            string chengxu;
            getline(cin, chengxu);
            
            if (chengxu == "1") {
                clear_screen();
                tic_tac_toe();
                Sleep(500);
                clear_screen();
                menu_stack.pop_back();
                continue;
            } else if (chengxu == "2") {
                clear_screen();
                bytebit();
                clear_screen();
                menu_stack.pop_back();
            } else if (chengxu == "3") {
                clear_screen();
                std::cout << "音乐" << endl;
                std::cout << "1.枫丹Fontaine  HOYO-MiX，宛迪萌" << endl;
                std::cout << "2.系ぎて rintaro soma" << endl;
                std::cout << "3.萤火虫の怨  闫东炜" << endl;
                std::cout << "4.MARENOL  LeaF&SigN_285" << endl;
                std::cout << "5.Luminescence  米虾Fomiki&初云Cloudies" << endl;
                std::cout << "6.RE Aoharu  Nor" << endl;
                std::cout << "7.填写此问卷以在下一个版本中添加歌曲" << endl;
                std::cout << "8.返回" << endl;
                
                string yingyue;
                getline(cin, yingyue);
                
                if (yingyue == "1") {
                    clear_screen();
                    play_sound(fontaine_path);
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "2") {
                    clear_screen();
                    play_sound(ji_path);
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "3") {
                    clear_screen();
                    play_sound(chongzi_path);
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "4") {
                    clear_screen();
                    play_sound(MARENOL_path);
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "5") {
                    clear_screen();
                    play_sound(Luminescence_path);
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "6") {
                    clear_screen();
                    play_sound(aoharu_path);
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "7") {
                    open_browser("https://forms.office.com/Pages/ResponsePage.aspx?id=024_FO1LrECXWUbEB3wI7-IEcxyPBj5Ljk6i0uUwfXdUMUM1Mkw2VFE5U0I2SzFTVjM4MjFSNUlQWi4u");
                    clear_screen();
                    menu_stack.pop_back();
                } else if (yingyue == "8") {
                    clear_screen();
                    menu_stack.pop_back();
                } else {
                    std::cout << "输入错误，请输入1,2,3或4。" << endl;
                    Sleep(500);
                    clear_screen();
                    continue;
                }
            } else if (chengxu == "4") {
                clear_screen();
                menu_stack.pop_back();
                continue;
            } else {
                std::cout << "输入无效，请重新输入，选择1、2、3或4。" << endl;
                clear_screen();
                continue;
            }
        }
        // Feedback menu
        else if (current_menu == "4") {
            clear_screen();
            open_browser("https://forms.office.com/Pages/ResponsePage.aspx?id=024_FO1LrECXWUbEB3wI7-IEcxyPBj5Ljk6i0uUwfXdUODlFT05ETk4zUkpUSDJFMUNIUDAyVktaMi4u");
            clear_screen();
            menu_stack.pop_back();
        }
        // Stop music
        else if (current_menu == "5") {
            stop_sound();
            clear_screen();
            menu_stack.pop_back();
        }
    }

    return 0;
}
}