#include <iostream>
#include <vector>

using namespace std;

struct coordinates{
    private:
        int x;
        int y;

    public:
        coordinates(): x(0), y(0){}
        coordinates(int _x, int _y): x(_x), y(_y){}

        int getX() { return x; }; //получение координаты X
        int getY() { return y; }; //получение координаты Y
};

//типы фигур
enum class pieceType{
    king, //король
    queen, //ферзь
    rook, //ладья
    bishop, //слон
    knight, //конь
    pawn //пешка
};

//цвет фигур
enum class color{
    black, //черный
    white //белый
};

class Piece{
    private:
        vector<coordinates> pattern;
        bool _isAlive = true; //состояние
        color _color; //цвет
        pieceType _type; //тип
        coordinates _pos; //координаты
        int _moves = 0;

    public:
        Piece(pieceType type): _pos(coordinates(0,0)), _color(color::white), _type(type){};
        Piece(coordinates pos, color col, pieceType type): _pos(pos), _color(col), _type(type){};

        //установить координаты фигуры
        void setPos(coordinates pos){
            _pos = pos;
        }

        void incMoves(){
            _moves++;
        }

        int getMoves(){
            return _moves;
        }

        bool isMoved(){
            if (_moves > 0) return true;
            return false;
        }

        virtual vector<coordinates> getPattern() = 0;

        //получение координаты фигуры
        coordinates getPos(){
            if (_isAlive)
                return _pos;
            else return coordinates(-1,-1);
        }

        //получить цвет фигуры
        color getColor(){
            return _color;
        }

        //получить тип фигуры
        pieceType getType(){
            return _type;
        }

        //пометка уничтожения фигуры
        void setDead(){
            _isAlive = false;
        }

        bool isAlive(){
            return _isAlive;
        }

        void printStats(){
            string type, color;
            switch (getType()){
                case pieceType::bishop:
                    type = "Слон";
                    break;

                case pieceType::king:
                    type = "Король";
                    break;

                case pieceType::knight:
                    type = "Конь";
                    break;

                case pieceType::pawn:
                    type = "Пешка";
                    break;

                case pieceType::queen:
                    type = "Ферзь";
                    break;

                case pieceType::rook:
                    type = "Ладья";
                    break;
            }

            switch (getColor()){
                case color::white:
                    color = "Белый";
                    break;
                
                case color::black:
                    color = "Черный";
                    break;
            }

            cout << "Тип фигуры: " << type << endl;
            cout << "Цвет фигуры: " << color << endl;
            cout << "Текущая позиция: " << getPos().getX() << getPos().getY() << endl;
            cout << "Живой - " << isAlive() << endl;
        }
};

class Pawn : public Piece{
    private:
        vector<coordinates> firstPattern = {
            coordinates(0, 1), coordinates(0, 2), coordinates(-1, 1), coordinates(1, 1)
        };
        
        vector<coordinates> pattern = {
            coordinates(0, 1), coordinates(-1, 1), coordinates(1, 1)
        };

    public:
        Pawn(): Piece(pieceType::pawn){}
        Pawn(coordinates pos, color col): Piece(pos, col, pieceType::pawn){}
        
        vector<coordinates> getPattern() override{
            if (isMoved()) return pattern;
            else return firstPattern;
        }
};

class Bishop : public Piece{
    private:
        vector<coordinates> pattern ={
            coordinates(-1,1), coordinates(1,1),
            coordinates(-1,-1), coordinates(1,-1),
        };

    public:
        Bishop(): Piece(pieceType::bishop){};
        Bishop(coordinates pos, color col): Piece(pos, col, pieceType::bishop){};

        vector<coordinates> getPattern() override{
            return pattern;
        }
};

class King: public Piece{
    private:
        vector<coordinates> pattern ={
            coordinates(-1,1), coordinates(0,1), coordinates(1,1),
            coordinates(-1,0), coordinates(1,0),
            coordinates(-1,-1), coordinates(0,-1), coordinates(1,-1)
        };

        vector<coordinates> bishopThreat ={
            coordinates(-1,1), coordinates(1,1),
            coordinates(-1,-1), coordinates(1,-1),
        };

    public:
        King(): Piece(pieceType::king){};
        King(coordinates pos, color col): Piece(pos, col, pieceType::king){};

        vector<coordinates> getPattern() override{
            return pattern;
        }
};

class Knight : public Piece{
    private:
        vector<coordinates> pattern = {
            coordinates(-1,2), coordinates(1,2),
            coordinates(2,1), coordinates(2,-1),
            coordinates(1,-2), coordinates(-1,-2),
            coordinates(-2,-1), coordinates(-2,1)
        };

    public:
        Knight(): Piece(pieceType::knight){};
        Knight(coordinates pos, color col): Piece(pos, col, pieceType::knight){};

        vector<coordinates> getPattern() override{
            return pattern;
        }
};

class Queen: public Piece{
    private:
        vector<coordinates> pattern ={
            coordinates(-1,1), coordinates(0,1), coordinates(1,1),
            coordinates(-1,0), coordinates(1,0),
            coordinates(-1,-1), coordinates(0,-1), coordinates(1,-1)
        };

    public:
        Queen(): Piece(pieceType::queen){};
        Queen(coordinates pos, color col): Piece(pos, col, pieceType::queen){};

        vector<coordinates> getPattern() override{
            return pattern;
        }
};

class Rook: public Piece{
    private:
        vector<coordinates> pattern ={
            coordinates(0,1), coordinates(1,0),
            coordinates(-1,0), coordinates(0,-1),
        };

    public:
        Rook(): Piece(pieceType::rook){};
        Rook(coordinates pos, color col): Piece(pos, col, pieceType::rook){};

        vector<coordinates> getPattern() override{
            return pattern;
        }
};

int main(){
    Piece *pawn = new Pawn();
    pawn->printStats();
}