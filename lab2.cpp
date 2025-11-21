#include <iostream>
#include <vector>
#include <memory>

using namespace std;

#define BOARD_MAX_X 7
#define BOARD_MAX_Y 7

//координаты клетки/фигуры
struct coordinates{
    private:
        int _x; //координата x
        int _y; //координата y

    public:
        coordinates(): _x(0), _y(0){}
        coordinates(int x, int y): _x(x), _y(y){}

        coordinates(const coordinates& c): _x(c._x), _y(c._y){}

        static coordinates setCoord(string str){
            if (str.length() == 2){
                int x = toupper(str[0])-'A';
                int y = str[1]-'0'-1;
                if (x>=0 && x<=7 && y>=0 && y<=7)
                    return coordinates(x,y);
                return coordinates(-1,-1);
            }
            if (str=="exit") return coordinates(8,8);
            return coordinates(-1,-1);
        }

        int getX() { return _x; }; //получение координаты X
        int getY() { return _y; }; //получение координаты Y

        //перегрузка оператора +
        coordinates operator+(const coordinates& coord) const{
            int x, y;
            x = this->_x + coord._x;
            y = this->_y + coord._y;
            return coordinates(x, y);
        }

        //перегрузка оператора -
        coordinates operator-(const coordinates& coord) const{
            int x, y;
            x = this->_x - coord._x;
            y = this->_y - coord._y;
            return coordinates(x, y);
        }

        //перегрузка оператора *
        coordinates operator* (const int& n) const{
            int x, y;
            x = this->_x * n;
            y = this->_y * n;
            return coordinates(x,y);
        }

        //перегрузка оператора ==
        bool operator==(const coordinates& coord) const{
            if (this->_x == coord._x && this->_y == coord._y)
                return true;
            return false;
        }

        //перегрузка оператора +=
        coordinates& operator+=(const coordinates coord){
            this->_x += coord._x;
            this->_y += coord._y;
            return *this;
        }

        //перегрузка оператора <<
        friend ostream& operator<<(std::ostream& os, const coordinates& c) {
            return os << static_cast<char>('A'+c._x) << c._y+1;
        }

        //перегрузка оператора <<
        friend istream& operator>>(std::istream& in, coordinates& c) {
            string str;
            in >> str;
            c = setCoord(str);
            return in;
        }

        //проверка принадлежности координат доске
        bool checkBound(){
            if (this->_x < 0 || this->_x > BOARD_MAX_X || this->_y < 0 || this->_y > BOARD_MAX_Y)
                return false;
            return true;
        }
};

//преобразование строки в координаты
coordinates convert(string& str){
    int x, y;
    if (str.length() == 2){
        x = toupper(str[0])-'A';
        y = str[1]-49;
        if (x>=0 && x<=7 && y>=0 && y<=7)
            return coordinates(x,y);
        return coordinates(-1,-1);
    }
    return coordinates(8, 8);
}

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

//Класс Фигура
class Piece{
    private:
        bool _isAlive = true; //состояние
        color _color; //цвет
        pieceType _type; //тип
        coordinates _pos; //координаты
        int _moves = 0; //кол-во ходов фигуры

    public:
        Piece(pieceType type): _pos(coordinates(0,0)), _color(color::white), _type(type){};
        Piece(coordinates pos, color col, pieceType type): _pos(pos), _color(col), _type(type){};

        //установить координаты фигуры
        void setPos(coordinates pos){
            _pos = pos;
        }

        //добавить ход
        void incMoves(){
            _moves++;
        }

        //получить ходы
        int getMoves(){
            return _moves;
        }

        //проверка передвижения
        bool isMoved(){
            if (_moves > 0) return true;
            return false;
        }

        //получение шаблона передвижения
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

        //проверка состояния
        bool isAlive(){
            return _isAlive;
        }

        //вывод информации
        void printInfo(){
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
            cout << "Текущая позиция: " << getPos() << endl;
            cout << "Живой - " << isAlive() << endl;
        }
};

//Класс Пешка
class Pawn: public Piece{
    private:
        vector<coordinates> _firstPattern = {
            coordinates(0, 1), coordinates(0, 2), coordinates(-1, 1), coordinates(1, 1)
        }; //шаблон первого хода
        
        vector<coordinates> _pattern = {
            coordinates(0, 1), coordinates(-1, 1), coordinates(1, 1)
        }; //шаблон хода, кроме первого

    public:
        Pawn(): Piece(pieceType::pawn){}
        Pawn(coordinates pos, color col): Piece(pos, col, pieceType::pawn){}
        
        //получение шаблона передвижения
        vector<coordinates> getPattern() override{
            if (isMoved()) return _pattern;
            else return _firstPattern;
        }
};

//Класс Слон
class Bishop: public Piece{
    private:
        vector<coordinates> _pattern ={
            coordinates(-1,1), coordinates(1,1),
            coordinates(-1,-1), coordinates(1,-1),
        }; //шаблон хода

    public:
        Bishop(): Piece(pieceType::bishop){};
        Bishop(coordinates pos, color col): Piece(pos, col, pieceType::bishop){};

        //получение шаблона передвижения
        vector<coordinates> getPattern() override{
            return _pattern;
        }
};

//Класс Король
class King: public Piece{
    private:
        vector<coordinates> _pattern ={
            coordinates(-1,1), coordinates(0,1), coordinates(1,1),
            coordinates(-1,0), coordinates(1,0),
            coordinates(-1,-1), coordinates(0,-1), coordinates(1,-1)
        }; //шаблон хода фигуры

    public:
        King(): Piece(pieceType::king){};
        King(coordinates pos, color col): Piece(pos, col, pieceType::king){};

        //получение шаблона передвижения
        vector<coordinates> getPattern() override{
            return _pattern;
        }
};

//Класс Конь
class Knight: public Piece{
    private:
        vector<coordinates> _pattern = {
            coordinates(-1,2), coordinates(1,2),
            coordinates(2,1), coordinates(2,-1),
            coordinates(1,-2), coordinates(-1,-2),
            coordinates(-2,-1), coordinates(-2,1)
        }; //шаблон хода фигуры

    public:
        Knight(): Piece(pieceType::knight){};
        Knight(coordinates pos, color col): Piece(pos, col, pieceType::knight){};

        //получение шаблона передвижения
        vector<coordinates> getPattern() override{
            return _pattern;
        }
};

//Класс Ферзь
class Queen: public Piece{
    private:
        vector<coordinates> _pattern ={
            coordinates(-1,1), coordinates(0,1), coordinates(1,1),
            coordinates(-1,0), coordinates(1,0),
            coordinates(-1,-1), coordinates(0,-1), coordinates(1,-1)
        }; //шаблон хода фигуры

    public:
        Queen(): Piece(pieceType::queen){};
        Queen(coordinates pos, color col): Piece(pos, col, pieceType::queen){};

        //получение шаблона передвижения
        vector<coordinates> getPattern() override{
            return _pattern;
        }
};

//Класс Ладья
class Rook: public Piece{
    private:
        vector<coordinates> _pattern ={
            coordinates(0,1), coordinates(1,0),
            coordinates(-1,0), coordinates(0,-1),
        }; //шаблон хода фигуры

    public:
        Rook(): Piece(pieceType::rook){};
        Rook(coordinates pos, color col): Piece(pos, col, pieceType::rook){};

        //получение шаблона передвижения
        vector<coordinates> getPattern() override{
            return _pattern;
        }
};

//Класс Клетка
class Square{
    private:
        shared_ptr<Piece> _piece; //фигура на клетке
        coordinates _pos; //координаты клетки

    public:
        Square(coordinates pos): _pos(pos) ,_piece(nullptr){};
        Square(coordinates pos, pieceType type, color col): _pos(pos){
            switch (type)
            {
            case pieceType::bishop:
                setPiece(make_shared<Bishop>(getPos(), col));
                break;
            
            case pieceType::king:
                setPiece(make_shared<King>(getPos(), col));
                break;

            case pieceType::knight:
                setPiece(make_shared<Knight>(getPos(), col));
                break;

            case pieceType::pawn:
                setPiece(make_shared<Pawn>(getPos(), col));
                break;

            case pieceType::queen:
                setPiece(make_shared<Queen>(getPos(), col));
                break;

            case pieceType::rook:
                setPiece(make_shared<Rook>(getPos(), col));
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
        void setPiece(shared_ptr<Piece> piece){
            if (_piece!=nullptr) _piece->setDead();
            _piece = move(piece);
            _piece->setPos(getPos());
        }

        //какая фигура стоит на клетке
        shared_ptr<Piece> getPiece(){
            return _piece;
        }

        Piece* getPiecePtr(){
            return _piece.get();
        }

        //убрать фигуру (при обычном ходе в начальной клетке)
        void removePiece(){
            _piece = nullptr;
        }

        //получить цвет фигуры
        color getPieceColor(){
            return _piece->getColor();
        }

        //получить тип фигуры
        pieceType getPieceType(){
            return _piece->getType();
        }

        //получить шаблон хода фигуры
        vector<coordinates> getPiecePattern(){
            return _piece->getPattern();
        }

        //проверить наличие фигуры на клетке
        bool hasPiece(){
            if (getPiece()==nullptr) return false;
            return true;
        }

        //получить доступные для фигуры на клетке ходы
        vector<coordinates> getMoves(shared_ptr<Square> board[8][8], color active){
            vector<coordinates> moves;
            switch (board[getPos().getX()][getPos().getY()]->getPieceType()){
                case pieceType::bishop:
                case pieceType::rook:
                case pieceType::queen:
                    moves = getContMoves(board, active);
                    break;

                case pieceType::knight:
                case pieceType::king:
                    moves = getShortMoves(board, active);
                    break;

                case pieceType::pawn:
                    moves = getPawnMoves(board, active);
                    break;
            }
            return moves;
        }

        //получить продолжительные ходы
        vector<coordinates> getContMoves(shared_ptr<Square> board[8][8], color active){
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

        //получить короткие ходы
        vector<coordinates> getShortMoves(shared_ptr<Square> board[8][8], color active){
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
        
        //получить ходы пешки
        vector<coordinates> getPawnMoves(shared_ptr<Square> board[8][8], color active){
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

        //вывод информации о клетке и фигуре, стоящей на ней
        void printInfo(){
            cout << "Клетка " << getPos() << endl;
            getPiece()->printInfo();
        }
};

//Класс Доска
class Board{
    private:
        color _activeColor; //текущий цвет
        pieceType _scheme[4][8] = {
            {pieceType::rook, pieceType::knight, pieceType::bishop, pieceType::queen, pieceType::king, pieceType::bishop, pieceType::knight, pieceType::rook},
            {pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, },
            {pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, pieceType::pawn, },
            {pieceType::rook, pieceType::knight, pieceType::bishop, pieceType::king, pieceType::queen, pieceType::bishop, pieceType::knight, pieceType::rook},
        }; //шаблон заполнения доски

        shared_ptr<Square> _board[8][8]; //доска

    public:
        Board(): _activeColor(color::white){
            initBoard();
        }

        //инициализация доски
        void initBoard(){
            for (int y=0; y<=7; y++){
                for (int x=0; x<=7; x++){
                    if (y<=1){
                        _board[x][y]=make_shared<Square>(coordinates(x, y), _scheme[y][x], color::white);
                    }
                    else if (y>=6){
                        _board[x][y]=make_shared<Square>(coordinates(x, y), _scheme[y-4][x], color::black);
                    }
                    else
                        _board[x][y]=make_shared<Square>(coordinates(x, y));
                }
            }
        }

        //вывод доски
        void drawBoard(){
            for (int y=7; y>=0; y--){
                cout << y+1;
                for (int x=0; x<=7; x++){
                    if (_board[x][y].get()->hasPiece()){
                        switch (_board[x][y].get()->getPiecePtr()->getType()){
                            case pieceType::pawn:
                                if (_board[x][y]->getPiecePtr()->getColor()==color::white)
                                    cout << u8"|\u2659 ";
                                else cout << u8"|\u265F ";
                                break;
                                
                            case pieceType::rook:
                                if (_board[x][y]->getPiecePtr()->getColor()==color::white)
                                    cout << u8"|\u2656 ";
                                else cout << u8"|\u265C ";
                                break;

                            case pieceType::knight:
                                if (_board[x][y]->getPiecePtr()->getColor()==color::white)
                                    cout << u8"|\u2658 ";
                                else cout << u8"|\u265E ";
                                break;

                            case pieceType::bishop:
                                if (_board[x][y]->getPiecePtr()->getColor()==color::white)
                                    cout << u8"|\u2657 ";
                                else cout << u8"|\u265D ";
                                break;

                            case pieceType::king:
                                if (_board[x][y]->getPiecePtr()->getColor()==color::white)
                                    cout << u8"|\u2654 ";
                                else cout << u8"|\u265A ";
                                break;

                            case pieceType::queen:
                                if (_board[x][y]->getPiecePtr()->getColor()==color::white)
                                    cout << u8"|\u2655 ";
                                else cout << u8"|\u265B ";
                                break;

                            default:
                                cout << " ";
                                break;
                        }
                    }
                    else
                        cout << "|  ";
                }
                cout << "|" << endl;
            }
            cout << "  A  B  C  D  E  F  G  H" << endl;
        }

        //отображение ходов фигуры
        bool drawMoves(coordinates pos){
            vector<coordinates> moves = getMoves(pos);
            if (!moves.empty()){
                for (int y=7; y>=0; y--){
                    cout << y+1;
                    for (int x=0; x<=7; x++){
                        bool found = false;
                        for (coordinates a : moves){
                            if (a==coordinates(x,y)){
                                cout << "| *";
                                found = true;
                                break;
                            }
                        }
                        if (!found) cout << "|  ";
                    }
                    cout << "|" << endl;
                }
                cout << "  A  B  C  D  E  F  G  H" << endl;
                getSquare(pos)->printInfo();
                return true;
            }
            return false;
        }

        //получение ходов фигуры по координатам
        vector<coordinates> getMoves(coordinates pos){
            return getSquare(pos)->getMoves(_board, _activeColor);
        }

        //получение клетки по координатам
        Square* getSquare(coordinates pos){
            return _board[pos.getX()][pos.getY()].get();
        }

        //проверка наличия фигуры на клетке
        bool sqHasPiece(coordinates pos){ 
            if (getSquare(pos)->hasPiece())
                return true;
            return false;
        }

        //получение фигуры на клетке
        shared_ptr<Piece> getSqPiece(coordinates pos){
            return getSquare(pos)->getPiece();
        }

        //смена текущего цвета
        void changeActive(){
            if (_activeColor == color::white) _activeColor = color::black;
            else _activeColor = color::white; 
        }

        //получение текущего цвета
        color getActive(){
            return _activeColor;
        }

        //проверка возможность сдвинуть выбранную фигуру
        bool checkOwner(coordinates pos){
            if (getSqPiece(pos)->getColor() == getActive()) return true;
            else return false;
        }

        //передвижение фигур
        int movePiece(coordinates c1, coordinates c2){
            int state = 0;
            if (!sqHasPiece(c1)){
                cout << "Поле пустое" << endl;
            }
            vector<coordinates> moves = getMoves(c1);
            bool found = false;
            for (coordinates a : moves){
                if (a==c2){
                    found = true;
                    break;
                }
            }
            if (found){
                if (sqHasPiece(c2) && getSqPiece(c2)->getType() == pieceType::king){
                    if (getActive() == color::white) state = 1;
                    else state = 2;
                }
                getSqPiece(c1)->incMoves();
                getSquare(c2)->setPiece(getSqPiece(c1));
                getSquare(c1)->removePiece();
                changeActive();
            }
            else cout << "Фигура не может сюда пойти" << endl;
            return state;
        }

};

//Класс Таймер
class Timer{
    private:
        time_t startTime; //время начала игры
        tm* gameTime; //длительность игры

    public:
        Timer(){
            startTimer();
        }
        
        //запуск таймера
        void startTimer(){
            startTime = time(NULL);
        }

        //остановка таймера
        void stopTimer(){
            time_t tmp = difftime(time(NULL), startTime);
            gameTime = localtime(&tmp);
        }

        //вывод продолжительности игры
        void printTime(){
            cout << "Время игры: " << gameTime->tm_hour-7 << ":" << gameTime->tm_min << ":" << gameTime->tm_sec << endl;
        }
};

//Класс Игрок
class Player{
    private:
        string _name; //имя
        int _wonGames; //кол-во побед
        int _lostGames; //кол-во поражений
        int _playedGames; //кол-во игр

    public:
        Player(string name): _name(name){
            clearStats();
        }

        //очистка статистики игрока
        void clearStats(){
            _wonGames = 0;
            _lostGames = 0;
            _playedGames = 0;
        }

        //установка имени игрока
        void setName(string name){
            _name = name;
        }

        //получение имени игрока
        string getName(){
            return _name;
        }

        //добавление выигранной игры
        void incWonGames(){
            _wonGames++;
            _playedGames++;
        }

        //добавление проигранной игры
        void incLostGames(){
            _lostGames++;
            _playedGames++;
        }

        void incPlayedGames(){
            _playedGames++;
        }

        //получение количества выигранных игр
        int getWonGames(){
            return _wonGames;
        }

        //получение количества проигранных игр
        int getLostGames(){
            return _lostGames;
        }

        //получение количества игр
        int getPlayedGames(){
            return _playedGames;
        }

        //вывод статистистики игрока
        void printStats(){
            cout << "Имя: " << getName() << endl;
            cout << "Кол-во побед: " << getWonGames() << endl;
            cout << "Кол-во поражений: " << getLostGames() << endl;
            cout << "Всего игр: " << getPlayedGames() << endl;
        }
};

//Класс Игра
class Game{
    private:
        shared_ptr<Player> whitePlayer; //игрок 1
        shared_ptr<Player> blackPlayer; //игрок 2
        shared_ptr<Board> board; //доска
        shared_ptr<Timer> timer; //таймер

    public:
        Game(): board(new Board()){
            string name1, name2;
            cout << "Введите имя 1 игрока: ";
            cin >> name1;

            cout << "Введите имя 2 игрока: ";
            cin >> name2;

            whitePlayer = make_shared<Player>(name1);
            blackPlayer = make_shared<Player>(name2);
        };

        //вывод результатов игры
        void printResults(int state){
            if (state == -1) cout << "Ничья!" << endl << endl;
            else if (state == 1) cout << "Белые победили!" << endl << endl;
            else if (state == 2) cout << "Черные победили!" << endl << endl;
            whitePlayer->printStats();
            cout << endl;
            blackPlayer->printStats();
            cout << endl;
            timer->printTime();
            cout << "Финальная позиция: " << endl;
            board->drawBoard();
        }

        //вывод текущего цвета
        void printActive(){
            color active = board->getActive();
            if (active == color::white) cout << "white: " << whitePlayer->getName() << endl;
            else cout << "black: " << blackPlayer->getName() << endl;
        }

        //запуск игры
        void play(){
            timer = make_shared<Timer>();
            int state = 0, prevWrong = false;
            do{
                system("clear");
                printActive();
                board->drawBoard();
                if (prevWrong) cout << "Ошибка" << endl;
                coordinates c1;
                cin >> c1;
                if (c1.checkBound()){
                    if (board->sqHasPiece(c1) && board->checkOwner(c1)){
                        bool hasMoves = board->drawMoves(c1);
                        if (hasMoves){
                            coordinates c2;
                            cin >> c2;
                            if (c2.checkBound())
                                state = board->movePiece(c1, c2);
                            else prevWrong = true;
                        }
                    }
                }
                else if (c1==coordinates(8,8)) state = -1;
                else prevWrong = true;
            }while (state == 0);
            system("clear");
            switch(state){
                case 1:
                    whitePlayer->incWonGames();
                    blackPlayer->incLostGames();
                    break;

                case 2:
                    blackPlayer->incWonGames();
                    whitePlayer->incLostGames();
                    break;

                case -1:
                    whitePlayer->incPlayedGames();
                    blackPlayer->incPlayedGames();
                    break;
            }

            timer->stopTimer();
            printResults(state);
        }
};


int main(){
    shared_ptr<Game> chess = make_shared<Game>();
    chess->play();
}
