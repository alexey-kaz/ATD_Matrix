#include <iostream>
using namespace std;

class Matr{
public:
    static int allocated;
    static int getAllocated() { return allocated; }
    virtual int getRow() const = 0;
    virtual int getColumn() const = 0;
    virtual void setRow(int i) = 0;
    virtual void setColumn(int j) = 0;
    virtual int& element(int i, int j) const = 0;
    virtual int * operator[] (int i) const = 0;
    friend ostream& operator<<(ostream &out, const Matr &matrix);
};

int Matr::allocated = 0;

ostream& operator<<(ostream &out, const Matr &matrix){
    for(int i=0; i<matrix.getRow(); i++)
        for(int j=0; j<matrix.getColumn(); j++){
            out << matrix.element(i,j);
            if ((j+1)%matrix.getColumn() == 0){
                cout << endl;
            } else {
                cout << " ";
            }
        }
    return out;
};

class Matrix : public Matr {
private:
    
    int **matrix;
    
    int m;
    
    int n;
    
protected:
    void Create(){
        matrix = new int*[m];
        allocated += m * sizeof(int*);
        for (int i=0; i<m; i++){
            matrix[i] = new int[n];
            allocated += n * sizeof(int);
        }
    } //создание матрицы
    
    void Destroy(){
        for (int i=0; i<m; i++){
            delete[] matrix[i];
            allocated -= n * sizeof(int);
        }
        delete[] matrix;
        allocated -= m * sizeof(int*);
    }
    
public:
    
    Matrix(int m_a = 1, int n_a = 1) :
    m(m_a), n(n_a) { Create(); }
    
    Matrix(const Matrix &matrix) :
    m(matrix.m), n(matrix.n) {
        Create();
        for(int i=0; i<matrix.m; i++){
            for(int j=0; j<matrix.n; j++){
                element(i,j) = matrix.element(i,j);
            }
        }
    } // Kонструктор копирования
    
    ~Matrix(){
        Destroy();
    }
    
    Matrix &operator=(const Matrix &matrix){
        if (this != &matrix){
            Destroy();
            m = matrix.getRow();
            n = matrix.getColumn();
            Create();
            for(int i=0; i < m; i++){
                for(int j=0; j < n; j++){
                    element(i,j) = matrix.element(i,j);
                }
            }
        }
        return *this;
    } // Перегрузка оператора присваивания
    
    Matrix operator* (const Matrix & matrix) const {
        Matrix tmp(m, matrix.n);
        for(int i = 0; i < tmp.m; i++)
            for(int j = 0; j < tmp.n; j++){
                tmp.element(i, j) = 0;
                for (int k = 0; k < n; k++){
                    tmp.element(i, j) += element(i, k) * matrix.element(k, j);
                }
            }
        return tmp;
    }; //Перемножение матриц
    
    int * operator[] (int i) const {
        return matrix[i];
    }
    
    void operator() (int i) const {
        for (int j = 0; j < n; j++)
            cout << (*this)[i][j] << ' ' ;
        cout << endl;
    }
    
    int getRow() const {
        return m;
    }
    
    int getColumn() const {
        return n;
    }
    
    void setRow(int i) {
        n = i;
    }
    
    void setColumn(int j) {
        m = j;
    }
    
    int& element(int i, int j) const {
        return matrix[i][j];
    } // вывести элемент матрицы
    
    void multBy(int x){
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                matrix[i][j] *= x;
    } //Умножить матрицу на число
    
    friend istream& operator>>(istream &in, Matrix &matrix);
};

class Square_Matrix : public Matrix{
public:
    Square_Matrix (int n) : Matrix(n,n) {}
    Square_Matrix(const Square_Matrix &matrix) {
        setRow(matrix.getRow());
        setColumn(matrix.getColumn());
        Create();
        for(int i=0; i<matrix.getRow(); i++){
            for(int j=0; j<matrix.getColumn(); j++){
                element(i,j) = matrix.element(i,j);
            }
        }
    } // Kонструктор копирования для квадратных матриц
};

istream& operator>>(istream &in, Matrix &matrix){
    for(int i=0; i<matrix.m; i++)
        for(int j=0; j<matrix.n; j++)
            in >> matrix.element(i,j);
    return in;
};

void reassignHelp (int number, Matrix &matrix) {
    cout << endl << number << "-ая матрица до преобразования: " << endl << matrix << endl;
    cout << "Введите размер новой матрицы через разделители" << endl;
    int i1, j1;
    cin >> i1 >> j1;
    Matrix tmpMatrix(i1, j1);
    cout << "Введите новую матрицу через разделители" << endl;
    cin >> tmpMatrix;
    matrix = tmpMatrix;
    cout << endl << number << "-ая матрица после перезаписи: " << endl << matrix << endl;
}

void elementHelp (int number, Matrix &matrix) {
    cout << endl << number << "-ая матрица: " << endl << matrix << endl;
    cout << "Введите строку и столбец элемента через разделители" << endl;
    int k, l;
    cin >> k >> l;
    if ((k <= matrix.getRow()) && (l <= matrix.getColumn())) {
        cout << endl << matrix.element(k-1, l-1) << endl << endl;
    } else {
        cout << "Выходит за границу матрицы" << endl << endl;
    }
}

void numberMultHelp (int number, Matrix &matrix) {
    cout << endl << number << "-ая матрица: " << endl << matrix << endl;
    cout << "Введите число, на которое нужно умножить матрицу" << endl;
    int k;
    cin >> k;
    matrix.multBy(k);
    cout << endl << number << "-ая матрица после умножения на число: " << endl << matrix << endl << endl;
}

void matrMultHelp (Matrix &matrix1, Matrix &matrix2){
    if (matrix1.getColumn() == matrix2.getRow()){
        matrix1 = matrix1 * matrix2;
        cout << endl << "Произведение: " << endl << matrix1 << endl << endl;
    } else {
        cout << "Перемножение невозможно, неправильная размерность" << endl;
    }
}

void matrMultDecide (int number2, Matrix &matrix1, Matrix &matrix2) {
    switch (number2) {
        case 1:
            matrMultHelp(matrix1, matrix1);
            break;
        case 2:
            matrMultHelp(matrix1, matrix2);
            break;
        default:
            cout << "Матрицы с номером " << number2 << " нет" << endl;
            break;
    }
}

int main() {
    cout << "Вызов инструкции - help \n";
    string str;
    cout << "По умолчанию обе матрицы имеют размерности 1x1, единственный элемент каждой равен 0 \n";
    Matrix first, second;
    first[0][0] = 0;
    second[0][0] = 0;
    cout << "Введите команду \n\n";
    cin >> str;
    while (!cin.eof()){
        if (str == "help") {
            cout << "\nreassign - перезаписать матрицу \ncopy - копирование одной матрицы в другую \nelement - вывод элемента строки i столбца j \nnumber_mult - умножение матрицы на число \nmatrix_mult - перемножение матриц \nallocated - кол-во использованной памяти в битах \nend - завершение работы\n\n";
        } else if (str == "reassign") {
            int matrixNumber;
            cout << "Введите номер матрицы, которую требуется перезаписать \n1 или 2 \n";
            cin >> matrixNumber;
            switch (matrixNumber) {
                case 1:
                    reassignHelp(matrixNumber, first);
                    break;
                case 2:
                    reassignHelp(matrixNumber, second);
                    break;
                default:
                    cout << "Матрицы с номером " << matrixNumber << " нет" << endl;
                    break;
            }
        } else if (str == "element") {
            cout << "Элемент какой матрицы требуется вывести?" << endl;
            int matrixNumber;
            cin >> matrixNumber;
            switch (matrixNumber) {
                case 1:
                    elementHelp(matrixNumber, first);
                    break;
                case 2:
                    elementHelp(matrixNumber, second);
                    break;
                default:
                    cout << "Матрицы с номером " << matrixNumber << " нет" << endl;
                    break;
            }
        } else if (str == "number_mult") {
            cout << "Какую матрицу умножаем на число?" << endl;
            int matrixNumber;
            cin >> matrixNumber;
            switch (matrixNumber) {
                case 1:
                    numberMultHelp(matrixNumber, first);
                    break;
                case 2:
                    numberMultHelp(matrixNumber, second);
                    break;
                default:
                    cout << "Матрицы с номером " << matrixNumber << " нет" << endl;
                    break;
            }
        } else if (str == "matrix_mult") {
            cout << endl << "Первая матрица: " << endl << first << endl;
            cout << "Вторая матрица: " << endl << second << endl;
            cout << "Какую матрицу умножаем на другую?" << endl << "Введите 1 1, если умножаем первую на первую" << endl << "Введите 1 2, если умножаем первую на вторую" << endl << "Введите 2 1, если умножаем вторую на первую" << endl << "Введите 2 2, если умножаем вторую на вторую" << endl ;
            int matrix1, matrix2;
            cin >> matrix1 >> matrix2;
            switch (matrix1) {
                case 1:
                    matrMultDecide(matrix2, first, second);
                    break;
                case 2:
                    matrMultDecide(matrix2, second, first);
                    break;
                default:
                    cout << "Матрицы с номером " << matrix1 << " нет" << endl;
                    break;
            }
        } else if (str == "allocated") {
            cout << "Использовано " << Matr::allocated << " бита" << endl << endl;
        } else if (str == "end") {
            cout << endl;
            break;
        } else {
            cout << "Неверная команда" << endl << endl;
        }
        cout << "Вызов инструкции - help \n";
        cout << "Введите команду \n\n";
        cin >> str;
    };
    cout << "До свидания!" << endl;
    return 0;
}
