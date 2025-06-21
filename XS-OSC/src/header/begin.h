#include <bits/stdc++.h>
using namespace std;
//Output logo(image)
static void l(string xxx)
{
    for(int i=0;i<xxx.length();i++)
    {
        switch(xxx[i])
        {
            case '0':std::cout<<"\033[40m  \033[0m";break;
            case '1':std::cout<<"\033[41m  \033[0m";break;
            case '2':std::cout<<"\033[42m  \033[0m";break;
            case '3':std::cout<<"\033[43m  \033[0m";break;
            case '4':std::cout<<"\033[44m  \033[0m";break;
            case '5':std::cout<<"\033[45m  \033[0m";break;
            case '6':std::cout<<"\033[46m  \033[0m";break;
            case '7':std::cout<<"\033[47m  \033[0m";break;
            case 'l':std::cout<<"\033[104m  \033[0m";break;
            case 'x':std::cout<<"\033[100m  \033[0m";break;
            default:std::cout<<"\033[0m  ";break;
        }
    }
            std::cout<<"\033[0m"<<endl;
}
static void begin(){
    cout << "\033c";
    srand(time(0));
    cout<<"\033[?25l";
    cout<<" ";
    l("00 00");
    cout<<" ";
    l("00 00");cout<<endl;
    cout<<" ";
    l("00 00");
    cout<<" ";
    l("00 00");
    Sleep(100);
    cout << "\033c";
    cout<<"\033[0m";
    cout<<" ";
    l("00 00");
    cout<<" ";
    l("0x x0");cout<<endl;
    cout<<" ";
    l("0x x0");
    cout<<" ";
    l("00 00");
    Sleep(100);
    cout << "\033c";
    cout<<"\033[0m";
    cout<<" ";
    l("xx xx");
    cout<<" ";
    l("xx xx");cout<<endl;
    cout<<" ";
    l("xx xx");
    cout<<" ";
    l("xx xx");
    Sleep(100);
    cout << "\033c";
    cout<<"\033[0m";
    cout<<" ";
    l("4x xl");
    cout<<" ";
    l("xx xx");cout<<endl;
    cout<<" ";
    l("xx xx");
    cout<<" ";
    l("lx x4");
    Sleep(100);
    cout << "\033c";
    cout<<"\033[0m";
    cout<<" ";
    l("44 ll");
    cout<<" ";
    l("44 ll");cout<<endl;
    cout<<" ";
    l("ll 44");
    cout<<" ";
    l("ll 44");
    cout << "XS-OS 4.0.0" << endl;
    Sleep(1000);
    cout << "Byteburst Studio Copyright";
    Sleep(2000);

}