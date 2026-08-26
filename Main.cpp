#include <Siv3D.hpp>
#include <vector>
#include <cmath>

class Game{
private:
    const Size screensize = {800 , 600};
    const Size BlockSize = {40 , 20};
    const int horiz = screensize.x / BlockSize.x , verti = screensize.y / BlockSize.y;
    std::vector<std::vector<int>> block_check;
    double Speed;
    const double Ball_Size = 5;
    double X , Y;
    double degree;
    int B_Y , B_X;
    const Font font{FontMethod::MSDF , 48};
    int clear_count = 0;
    int mode = 0;
    Size Under_Bar_Size;
    Size mouse_pos;
    int clear_text_move = 0;
public:
    Game(){
        Window::Resize(screensize);
        Scene::SetBackground(HSV{0 , 0 , 1.0});
    }
    int Init(){
        block_check.resize(verti , std::vector<int>(horiz , 0));
        for(int i = 2; i < 6; ++i){
            for(int j = 0; j < horiz; ++j){
                block_check[i][j] = 1;
                clear_count++;
            }
        }
        return clear_count;
    }
    void Draw(){
        ClearPrint();
        for(int i = 2; i < 6; ++i){
            for(int j = 0; j < horiz; ++j){
                if(block_check[i][j] == 1){
                    Rect{j * BlockSize.x , i * BlockSize.y , BlockSize.x - 1 , BlockSize.y - 1}.draw(HSV{150 + (i * 30) , 1.0 , 1.0});
                }
            }
        }
        Rect{mouse_pos.x - (Under_Bar_Size.x / 2) , screensize.y - 50 , Under_Bar_Size.x , Under_Bar_Size.y}.draw(Palette::Blue);
        Circle{X , Y , Ball_Size}.draw(Palette::Skyblue);
        //Print << B_X << ',' << B_Y;
        //Print << degree;
        //Print << clear_count;
    }
    
    int Update(){
        if(Y > screensize.y){
            mode = 0;
        }
        if(MouseL.down()){
            mode++;
        }
        if(mode == 0){
            font(U"Push Game Start!").draw(15 , Vec2{340,400}, ColorF{ 0.2 });
            X = 400.0; Y = 500.0; //ボール初期位置
            degree = 90;
            Speed = 0.0;
            Under_Bar();
        }else{
            Speed = 10.0;
            double radian = degree * M_PI /180;
            Y += (sin(radian) * Speed);
            X += (cos(radian) * Speed);
            if(X < 0 || X > screensize.x) degree = 180 - degree;
            if(Y < 0) degree = -(degree);
            Block_Check(radian);
            Under_Bar();
        }
        return clear_count;
    }

    void Under_Bar(){
        Under_Bar_Size = {80 , 5};
        mouse_pos = Cursor::Pos();
        if(mode == 0) mouse_pos.x = screensize.x / 2;
        if(mouse_pos.x < Under_Bar_Size.x / 2){
            mouse_pos.x = Under_Bar_Size.x / 2;
        }
        if(mouse_pos.x > screensize.x - (Under_Bar_Size.x / 2)){
            mouse_pos.x = screensize.x - (Under_Bar_Size.x / 2);
        }
        if(Y < screensize.y - 45 && Y > screensize.y - 55 && X < mouse_pos.x + Under_Bar_Size.x / 2 && X > mouse_pos.x - Under_Bar_Size.x / 2){
            double bound = (X - (mouse_pos.x - (Under_Bar_Size.x / 2))) / Under_Bar_Size.x;//ボールの位置がバーの何％の位置にいるかを確認
            if(bound < 0.2) bound = 0.2;
            if(bound > 0.8) bound = 0.8;
            degree = 180 - (180 * -(bound));
        }
    }
    
    void Block_Check(double &radian){
        B_Y = (int)(Y / BlockSize.y); B_X = (int)(X / BlockSize.x);
        if(B_Y >= 0 && B_Y < verti && B_X >= 0 && B_X < horiz){
            if(block_check[B_Y][B_X] == 1){
                //1step前の位置を確認※この後の当たり判定処理で、縦方向で当たったのか横方向で当たったのかを判定する
                int C_Y = (int)((Y - (sin(radian) * Speed)) / BlockSize.y), C_X = (int)((X - (cos(radian) * Speed)) / BlockSize.x);
                if(C_X < 0 || C_X > horiz - 1){
                    degree = -(degree);
                }else{
                    //縦方向のブロック当たり判定
                    if(C_Y > 0 && C_Y < verti - 1){
                        if(block_check[C_Y - 1][C_X] == 1 || block_check[C_Y + 1][C_X] == 1) degree = -(degree);
                    }
                    else if(C_Y > 0){
                        if(block_check[C_Y - 1][C_X] == 1) degree = -(degree);
                    }
                    else if(C_Y < verti - 1){
                        if(block_check[C_Y + 1][C_X] == 1) degree = -(degree);
                    }
                    //横方向のブロック当たり判定
                    if(C_X > 0 && C_X < horiz - 1){
                        if(block_check[C_Y][C_X - 1] == 1 || block_check[C_Y][C_X + 1] == 1) degree = 180 - degree;
                    }else if(C_X > 0){
                        if(block_check[C_Y][C_X - 1] == 1) degree = 180 - degree;
                    }else if(C_X < horiz - 1){
                        if(block_check[C_Y][C_X + 1] == 1) degree = 180 - degree;
                    }
                }
                
                block_check[B_Y][B_X] = 0;
                clear_count--;
            }
        }
    }
    
    void Clear(){
        if(clear_text_move <= 150) clear_text_move++;
        font(U"GAME GLEAR!").draw(80 , Vec2{115,clear_text_move}, ColorF{ 0.2 });
    }
};

void Main(){
    Game game;
    int clear_count = game.Init();
    while(System::Update()){
        if(clear_count > 0){
            clear_count = game.Update();
            game.Draw();
        }else{
            game.Clear();
        }
    }
}
