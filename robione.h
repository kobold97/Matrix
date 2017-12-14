#include <iostream>
#include <iomanip>
#include <fstream>

class CMatrix
{
    struct matrix;
    matrix * pointer;
public:
    CMatrix(int, int, float, float);
    CMatrix(CMatrix&);
    CMatrix(std::fstream& );
    ~CMatrix();
    class Cref;
    CMatrix operator*(const CMatrix&) const;
    CMatrix& operator=(const CMatrix&);
    friend std::ostream& operator<<(std::ostream&, const CMatrix&);
    class WrongDim {};
    class BadIndex {};
    Cref operator[] (int);
    void write(int,int, float);

};


struct CMatrix::matrix
{
    int row, col;
    float **mat;
    int n, i;

    matrix(int nrow, int ncol)
    {
        row = nrow;
        col = ncol;
        n = 1;
        mat = new float*[row];
        try
        {
            for( i = 0; i < row; ++i)
                mat[i] = new float[col];
        }
        catch(std::bad_alloc)
        {
            for (int j=0; j<i; j++)
                delete [] mat[j];
            delete [] mat;

        }
    }

    ~matrix()
    {
        for(int i = 0; i<row; i++)
            delete [] mat[i];
        delete [] mat;
    };

    matrix * detach()
    {
        if(n==1) return this;
        matrix* t = new matrix(row, col);
        for(int i=0; i<row; i++)
        {
            for(int j=0; j<col; j++)
            {
                t->mat[i][j]=mat[i][j];
            }
        }
        n--;
        return t;
    }
};

class CMatrix::Cref
{
public:
    friend class CMatrix;
    CMatrix& m;
    int row, col;

    Cref(CMatrix& mm, int ii, int jj): m(mm),  row(ii), col(jj)
    {
    };

    Cref operator[](int ncol)
    {
        col=ncol;
        if((m.pointer->row<=row) || (m.pointer->col<=col))
        {
            throw BadIndex();
        }
        return *this;
    }

    operator float() const
    {
        return m.pointer->mat[row][col];
    }

    Cref& operator = (float num)
    {
        m.write(row,col,num);
        return *this;
    }
};

CMatrix::Cref CMatrix::operator[] (int row)
{
    return Cref(*this, row, 0);
}

CMatrix::CMatrix(int row=0, int col=0, float cross = 0, float rest = 0)
{
    pointer = new matrix(row, col);

    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            if(i==j)
                pointer->mat[i][j]=cross;
            else
                pointer->mat[i][j]=rest;
        }
    }
}


CMatrix::CMatrix(CMatrix & src)
{
    src.pointer->n++;
    pointer = src.pointer;
}

CMatrix::CMatrix(std::fstream& file)
{
    int col=0, row=0;
    file >> row >> col;
    pointer = new matrix(row, col);
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            file >> pointer->mat[i][j];
        }
    }
}

CMatrix::~CMatrix()
{
    if(--pointer->n == 0)
    {
        delete pointer;
    }
}

CMatrix CMatrix::operator*(const CMatrix& src) const
{
    if(pointer->col!=src.pointer->row)
        throw WrongDim();
    CMatrix result(pointer->row,src.pointer->col,0,0);
    float sum = 0;
    for(int i = 0; i<pointer->row; i++)
    {
        for(int j = 0; j < src.pointer->col; j++)
        {
            for(int k = 0; k <src.pointer->row; k++)
            {
                sum += pointer->mat[i][k]*src.pointer->mat[k][j];
            }
            result.pointer->mat[i][j] = sum;
            sum = 0;
        }
    }
    return result;
}

CMatrix& CMatrix::operator=(const CMatrix& src)
{
    src.pointer->n++;
    if(--pointer->n == 0)
        delete pointer;
    pointer = src.pointer;
    return *this;
}

std::ostream& operator << (std::ostream& o, const CMatrix& src)
{
    for(int i=0; i<src.pointer->row; i++)
    {
        for(int j=0; j<src.pointer->col; j++)
            o  << src.pointer->mat[i][j] << "  ";
        o << std::endl;
    }
    return o;
}

void CMatrix::write(int i,int j, float n)
{
    pointer = pointer->detach();
    pointer->mat[i][j] = n;
}
// A moze by tak dodac dodawanie macierzy
// Tak to zdecydowanie dobry pomysl
