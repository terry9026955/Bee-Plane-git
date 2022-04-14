#include <iostream>
#include <windows.h>
#include <iomanip> // setw()
#include <conio.h>

using namespace std;

void gotoxy(int xpos, int ypos){
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos; scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput,scrn);
}

void ShowConsoleCursor(bool showFlag){  //remove blinking cursor
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

//蜜蜂類別
class Bee{
public:
    int x, y, lastx, lasty, counter = 0, direction = 1;
    Bee *next = NULL;    // 預設NULL
    Bee *prev = NULL;
    Bee(int a, int b):x(a), y(b), lastx(a), lasty(b){}
    void move(){
    	x += direction;

    	counter++;
    	if(counter % 20 == 0)
			direction *= -1;

         // 照要連續顯示多個的需求來看，清掉上一步應該要是跟著move階段執行
         // 然後讓show專心處理繪製當前狀態

         // 在while()中
         // 讓所有物件統一執行先move=>clearPath ，這樣就統一完成更新當前狀態，並清掉上次狀態
         // 這時統一show的時候就不會有覆蓋問題


		clearPath();
    }
    void clearPath(){
        gotoxy(lastx, lasty);
        cout << " ";

        lastx = x;
        lasty = y;
	}

    void show(){

        gotoxy(x, y);
        cout << "*";
    }

    int get_B_x(){
        return x;
    }

    int get_B_y(){
        return y;
    }
};

//飛機類別
class Plane{
protected:
    int px, py, lastpx, lastpy;
public:
    Plane(){}      //初始建構子
    Plane(int a, int b):px(a), py(b), lastpx(a), lastpy(b){
        gotoxy(px, py);
        cout << "A";
    }

    void p_show(){
        gotoxy(px, py);
        cout << "A";
    }

    void p_move(char input){
        switch(input){
            case 'a':
                if(px <= 1){    //左右移動範圍為1~24
                    px = px;
                    break;
                }else{
                    px = px - 1;
                    break;
                }
            case 'd':
                if(px >= 24){
                    px = px;
                    break;
                }else{
                    px = px + 1;
                    break;

                }
        }
        p_clearPath();
    }

    void p_clearPath(){
        gotoxy(lastpx, lastpy);
        cout << " ";
        lastpx = px;
        lastpy = py;
    }

    /*int catchPlane_xy(){     //抓飛機的Y座標
        return py;
    }*/

    int nowpx(){        //回傳XY座標給飛機子彈做建構子
        return px;
    }
    int nowpy(){
        return py;
    }

};


class Plane_bullet{
    int bx, by, lastby;
public:
    Plane_bullet *next;
    Plane_bullet *prev;
    Plane_bullet(int a, int b):bx(a), by(b){
        lastby = by;
        next = prev = NULL;
    }

    void Plane_bullet_move(){
        by = by - 1;
        Clear_Plane_bullet();
    }

    void Clear_Plane_bullet(){
        gotoxy(bx, lastby);
        cout << " ";
        lastby = by;
    }

    void show_Plane_bullet(){
        if(by == 0){
            gotoxy(bx, by);
            cout << " ";
        }else{
            gotoxy(bx, by);
            cout << "^";
        }
    }

    int get_PB_bx(){
        return bx;
    }
    int get_PB_by(){
        return by;
    }

};

class Score{
    int score = 0;
    int bullet_firetimes = 0;
public:
    Score(){
        gotoxy(50, 10);
        cout << "Now you have: " << score << " points";
    }
    void showScore(){
        gotoxy(50,10);
        cout << "Now you have: " << score << " points";
        gotoxy(50,12);
        cout << "You fire " << bullet_firetimes << " times";
    }
    void pluspoint(){
        score += 10;
    }
    void bulletplus(){
        bullet_firetimes = bullet_firetimes + 1;
    }

};



int main(){
    system("CLS");

    ShowConsoleCursor(false);   //remove blinking underscore on console/ CMD prompt
	//cout << "0123456789012345";
    Score score;

    Bee *b1 = new Bee(0, 1);            //建立所有Bee物件
    Bee *b2 = new Bee(1, 1);
    Bee *b3 = new Bee(2, 1);
    Bee *b4 = new Bee(3, 1);
    Bee *b5 = new Bee(4, 1);
    Bee *b6 = new Bee(0, 2);
    Bee *b7 = new Bee(1, 2);
    Bee *b8 = new Bee(2, 2);
    Bee *b9 = new Bee(3, 2);
    Bee *b10 = new Bee(4, 2);

    Bee *p = b1;                        //Linked List去串所有Bee
    b1->next = b2;
    b2->next = b3;
    b3->next = b4;
    b4->next = b5;
    b5->next = b6;
    b6->next = b7;
    b7->next = b8;
    b8->next = b9;
    b9->next = b10;

    b1->prev = NULL;
    b2->prev = b1;
    b3->prev = b2;
    b4->prev = b3;
    b5->prev = b4;
    b6->prev = b5;
    b7->prev = b6;
    b8->prev = b7;
    b9->prev = b8;
    b10->prev = b9;

    Plane *ppp = new Plane(10,10);                   //宣告飛機

    Plane_bullet *pbp = NULL;
    Plane_bullet *last = NULL;

    int coco = 0;   //Bee的週期累計

    while(1){
        coco++;
        score.showScore();

        //控制飛機左右移動
        if(kbhit()){
            char input = getch();
            if(input == 'a' || input == 'd'){
                ppp->p_move(input);
                ppp->p_show();
            }else if(input == ' '){
                int a = ppp->nowpx();
                int b = ppp->nowpy();
                if(pbp == NULL){
                    pbp = new Plane_bullet(a, b-1);
                    score.bulletplus();
                    last = pbp;
                }
                else{
                    last->next = new Plane_bullet(a, b-1);
                    score.bulletplus();
                    last->next->prev = last;
                    last = last->next;
                }
            }
        }
        Plane_bullet *pbit = pbp;
        while(pbit != NULL){            //每顆子彈移動一次，接著換下一顆子彈動一次，全動完之後會再run一次大迴圈到by=0
            //cout<<"bullet\n";
            pbit->Plane_bullet_move();
            //pbit->Clear_Plane_bullet();
            pbit->show_Plane_bullet();
            pbit = pbit->next;
        }

        pbit = pbp;
        while(pbit != NULL){
            if(!pbit->get_PB_by()){     //pbit的by座標等於0的時候
                pbit->Clear_Plane_bullet();     //清空他
                if(pbit->prev!=NULL)
                    pbit->prev->next=pbit->next;
                if(pbit->next!=NULL)
                    pbit->next->prev=pbit->prev;
                Plane_bullet *now=pbit;
                pbit=pbit->next;
                if(now==last) last = now->prev;
                if(now==pbp) pbp = pbp->next;
                delete now;
            }else{
                pbit=pbit->next;    //子彈跳下一個物件繼續執行上述動作檢查
            }
        }

        while(pbit != NULL){
            p = b1;
            if((pbit->get_PB_bx() == p->get_B_x()) && (pbit->get_PB_by() == p->get_B_y())){ //重疊的話，刪除這個p物件(敵人)
                p->clearPath();
                p->prev->next = p->next;
                p->next->prev = p->prev;
                p = p->next;
                delete p;

                pbit->Clear_Plane_bullet();     //清空子彈
                if(pbit->prev!=NULL)
                    pbit->prev->next=pbit->next;
                if(pbit->next!=NULL)
                    pbit->next->prev=pbit->prev;
                Plane_bullet *now=pbit;
                pbit=pbit->next;
                if(now==last) last = now->prev;
                if(now==pbp) pbp = pbp->next;
                delete now;

                score.pluspoint();  //加分
            }else{
                p = p->next;
            }
            //p = p->next;    //如果沒重疊，那p就跳下去進行上述動作檢查座標
        }

        //Bee自動移動的部分:
        // 先show在move的話，會變成顯示每次都是顯示更新前的狀態
        // 先move在show, 這樣才不會讓Bee物件互相衝突被覆蓋到

        if(coco >= 10){     //while迴圈run10次，Bee就自己動1次
            p = b1;
            while(p!=NULL){
                p->move();
                p = p->next;
            }

            p = b1;
            while(p!=NULL){
                p->show();
                p = p->next;
            }
            coco = 0;
        }
        p = b1;







        // debug

        /*p = b1;
        for(int i = 0; p != NULL; i++){
	        gotoxy(0, 5 + i);
	        cout << "bee" << i+1 << ":" << setw(2) << p->x;
            p = p->next;
        }*/
    	Sleep(30);  //0.03秒的週期

    }
}
