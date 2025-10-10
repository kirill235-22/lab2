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

class Square{
    private:
        Piece* _piece; //фигура на клетке
        coordinates _pos; //координаты клетки

    public:
        Square(coordinates pos): _pos(pos) ,_piece(nullptr){};
        Square(coordinates pos, pieceType type, color col): _pos(pos){
            switch (type)
            {
            case pieceType::bishop:
                setPiece(new Bishop(getPos(), col));
                break;
            
            case pieceType::king:
                setPiece(new King(getPos(), col));
                break;

            case pieceType::knight:
                setPiece(new Knight(getPos(), col));
                break;

            case pieceType::pawn:
                setPiece(new Pawn(getPos(), col));
                break;

            case pieceType::queen:
                setPiece(new Queen(getPos(), col));
                break;

            case pieceType::rook:
                setPiece(new Rook(getPos(), col));
                break;

            default:
                removePiece();
                break;
            }
        }

        //получить координаты клетки
        coordinates getPos(){
            return _pos;
        }

        //поменять фигуру (при срубе или в конечной клетке)
        void setPiece(Piece* piece){
            if (_piece!=nullptr) _piece->setDead();
            _piece = piece;
            _piece->setPos(getPos());
        }

        //какая фигура стоит на клетке
        Piece* getPiece(){
            return _piece;
        }

        //убрать фигуру (при обычном ходе в начальной клетке)
        void removePiece(){
            _piece = nullptr;
        }

        color getPieceColor(){
            return _piece->getColor();
        }

        pieceType getPieceType(){
            return _piece->getType();
        }

        vector<coordinates> getPiecePattern(){
            return _piece->getPattern();
        }

        bool hasPiece(){
            if (getPiece()!=nullptr) return true;
            return false;
        }

        //получить доступные для фигуры на клетке ходы
        vector<coordinates> getMoves(Square* board[8][8], color active){
            vector<coordinates> moves;
            switch (board[getPos().getX()][getPos().getY()]->getPieceType()){
                case pieceType::bishop:
                case pieceType::rook:
                case pieceType::queen:
                    moves = getContMoves(board, active);
                    break;

                case pieceType::king:
                    moves = getShortMoves(board, active);
                    break;

                case pieceType::knight:
                    moves = getShortMoves(board, active);
                    break;

                case pieceType::pawn:
                    moves = getPawnMoves(board, active);
                    break;
            }
            return moves;
        }

        vector<coordinates> getContMoves(Square* board[8][8], color active){
            vector<coordinates> moves, pattern;
            coordinates pos = getPos();
            pattern = getPiecePattern();
            for (coordinates move : pattern){
                int n=1;
                bool stop=false;
                while (!stop){
                    coordinates tpos = pos + move*n;
                    if (tpos.checkBound()){
                        if (!board[tpos.getX()][tpos.getY()]->hasPiece()){
                            moves.push_back(tpos);
                            n++;
                        }
                        else if (board[tpos.getX()][tpos.getY()]->getPieceColor()!=active){
                            moves.push_back(tpos);
                            stop = true;
                        }
                        else stop = true;
                    }
                    else stop=true;
                }
            }
            return moves;
        }

        vector<coordinates> getShortMoves(Square* board[8][8], color active){
            vector<coordinates> moves, pattern;
            coordinates pos = getPos();
            pattern = getPiecePattern();
            for (coordinates move : pattern){
                coordinates tpos = pos + move;
                    if (tpos.checkBound()){
                        if (!board[tpos.getX()][tpos.getY()]->hasPiece() || board[tpos.getX()][tpos.getY()]->getPieceColor()!=active)
                            moves.push_back(tpos);
                    }
            }
            return moves;
        }
        
        vector<coordinates> getPawnMoves(Square* board[8][8], color active){
            vector<coordinates> moves, pattern;
            coordinates pos = getPos(), tpos, forward;
            pattern = getPiecePattern();
            for (coordinates move : pattern){
                if (getPieceColor()==color::white) {
                    tpos = pos + move;
                    forward = pos + coordinates(0, 1);
                }
                else {
                    tpos = pos - move;
                    forward = pos - coordinates(0, 1);
                }
                if (tpos.checkBound()){
                    if (move.getX()==0){
                        if (!board[forward.getX()][forward.getY()]->hasPiece())
                            moves.push_back(tpos);
                    }
                    else{
                        if (board[tpos.getX()][tpos.getY()]->hasPiece() && board[tpos.getX()][tpos.getY()]->getPieceColor()!=active)
                            moves.push_back(tpos);
                    }
                }
            }
            return moves;
        }

};

int main(){
    Piece *pawn = new Pawn();
    pawn->printStats();
}