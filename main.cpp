#include <iostream>
#include <fstream>

using namespace std;
ifstream fin("abce.in");
ofstream fout("abce.out");

struct nod
{
    long key;
    bool culoare;
    nod* p;
    nod* fd;
    nod* fs;
    nod():key(0),culoare(0),p(nullptr),fd(nullptr),fs(nullptr) {}
    nod(long a,bool b,nod* c=nullptr,nod* d=nullptr,nod* e=nullptr):key(a),culoare(b),p(c),fd(d),fs(e) {}
    nod(const nod& n)
    {
        key=n.key;
        culoare=n.culoare;
        p=n.p;
        fd=n.fd;
        fs=n.fs;
    }
    ~nod()
    {
        key=0;
        culoare=0;
        if(fs!=nullptr)
            delete fs;
        if(fd!=nullptr)
            delete fd;
    }
    void inlocuire(nod* fiu)
    {
        if(fiu!=nullptr)
            fiu->p=this->p;
        if(this->p!=nullptr&&this==this->p->fs)
            this->p->fs=fiu;
        else if(this->p!=nullptr)
            this->p->fd=fiu;
    }
    void rotatie_dreapta()
    {
        nod* aux=this->fs;
        this->fs=aux->fd;
        if(aux->fd!=nullptr)
        {
            aux->fd->p=this;
        }
        aux->p=this->p;
        if(this->p!=nullptr)
        {
            if(this==this->p->fs)
                this->p->fs=aux;
            else
                this->p->fd=aux;
        }
        aux->fd=this;
        this->p=aux;
    }
    void rotatie_stanga()
    {
        nod* aux=this->fd;
        this->fd=aux->fs;
        if(aux->fs!=nullptr)
        {
            aux->fs->p=this;
        }
        aux->p=this->p;
        if(this->p!=nullptr)
        {
            if(this==this->p->fs)
                this->p->fs=aux;
            else
                this->p->fd=aux;
        }
        aux->fs=this;
        this->p=aux;
    }

};



struct arbori_rosu_negru
{
    nod* start= new nod;
    arbori_rosu_negru():start(nullptr) {}
    ~arbori_rosu_negru()
    {
        delete start;
    }
    void inserare(int x)
    {
        nod* ptr=new nod;
        ptr->culoare=1;
        ptr->key=x;
        if(start==nullptr)
        {
            start=ptr;
            start->culoare=0;
        }
        else
        {
            nod* i=this->start;
            int ok=1;
            while(ok==1)
            {
                if(i->key<x)
                    if(i->fd!=nullptr)
                        i=i->fd;
                    else
                    {
                        i->fd=ptr;
                        ptr->p=i;
                        break;
                    }
                else
                {
                    if(i->fs!=nullptr)
                        i=i->fs;
                    else
                    {
                        i->fs=ptr;
                        ptr->p=i;
                        break;
                    }
                }
            }
        }
        while(ptr!=start&&ptr->p->culoare==1)
        {
            if(ptr->p==ptr->p->p->fs)
            {
                nod* unch=ptr->p->p->fd;
                if(unch!=nullptr&&unch->culoare==1)
                {
                    ptr->p->culoare=0;
                    unch->culoare=0;
                    ptr->p->p->culoare=1;
                    ptr=ptr->p->p;
                }
                else
                {
                    if(ptr==ptr->p->fd)
                    {
                        ptr=ptr->p;
                        ptr->rotatie_stanga();
                        if(start==ptr)
                            start=ptr->p;
                    }

                    ptr->p->culoare=0;
                    ptr->p->p->culoare=1;
                    nod* b=ptr->p->p;
                    b->rotatie_dreapta();
                    if(b==start)
                        start=b->p;


                }
            }
            else
            {
                nod* unch=ptr->p->p->fs;
                if(unch!=nullptr&&unch->culoare==1)
                {
                    ptr->p->culoare=0;
                    unch->culoare=0;
                    ptr->p->p->culoare=1;
                    ptr=ptr->p->p;
                }
                else
                {
                    if(ptr==ptr->p->fs)
                    {
                        ptr=ptr->p;
                        ptr->rotatie_dreapta();
                        if(start==ptr)
                            start=ptr->p;
                    }

                    ptr->p->culoare=0;
                    ptr->p->p->culoare=1;
                    nod* b=ptr->p->p;
                    b->rotatie_stanga();
                    if(b==start)
                        start=b->p;


                }
            }
            start->culoare=0;
        }

    }
    void stergere(long);
    bool find_val(long x)
    {
        nod* i=start;
        while(i!=nullptr)
        {
            if(i->key<x)
                i=i->fd;
            else if(i->key>x)
                i=i->fs;
            else if(i->key==x)
                return 1;
        }
        return 0;
    }
    int predecesor(long x)
    {
        nod* i=start;
        long y=-1;
        while(i!=nullptr)
        {
            if(i->key>x)
                i=i->fs;
            else if(i->key<x)
            {
                y=i->key;
                i=i->fd;
            }
            else
                return i->key;

        }
        return y;
    }
    int succesor(long x)
    {
        nod* i=start;
        long y=-1;
        while(i!=nullptr)
        {
            if(i->key<x)
                i=i->fd;
            else if(i->key>x)
            {
                y=i->key;
                i=i->fs;
            }
            else
                return i->key;

        }
        return y;
    }
    nod* ptr_pred(long x)
    {
        nod* i=start;
        nod* y=nullptr;
        while(i!=nullptr)
        {
            if(i->key>x)
                i=i->fs;
            else if(i->key<x)
            {
                y=i;
                i=i->fd;
            }
            else
                return i;

        }
        return y;
    }
    nod* ptr_succe(long x)
    {
        nod* i=start;
        nod* y=nullptr;
        while(i!=nullptr)
        {
            if(i->key<x)
                i=i->fd;
            else if(i->key>x)
            {
                y=i;
                i=i->fs;
            }
            else
                return i;

        }
        return y;
    }
    void interval(long x, long y)
    {
        long i=succesor(x);
        while(i!=-1&&i<=y)
        {
            fout<<i<<" ";
            i=succesor(i+1);
        }

        fout<<"\n";
    }
};

void strg_c1(nod*,arbori_rosu_negru &);
void strg_c2(nod*,arbori_rosu_negru &);
void strg_c3(nod*,arbori_rosu_negru &);
void strg_c4(nod*,arbori_rosu_negru &);
void strg_c5(nod*,arbori_rosu_negru &);
void strg_c6(nod*,arbori_rosu_negru &);
void strg_un_fiu(nod*,arbori_rosu_negru &);

void arbori_rosu_negru::stergere(long val)
    {
        nod* n=ptr_pred(val);
        if(n==nullptr)
            return;
        if(n->key!=val)
            return;
        if(n->fd!=nullptr&&n->fs!=nullptr)
        {
            nod*aux=ptr_pred(n->key-1);
            long i=aux->key;
            aux->key=n->key;
            n->key=i;
            n=aux;
        }
        strg_un_fiu(n,*this);

    }
void strg_c6(nod* n, arbori_rosu_negru &ar)
{
    nod* frate;
    if(n->p->fs==n)
        frate=n->p->fd;
    else
        frate=n->p->fs;
    frate->culoare=n->p->culoare;
    n->p->culoare=0;
    if(n==n->p->fs)
    {
        if(frate->fd!=nullptr)
            frate->fd->culoare=0;
        nod* parinte=n->p;
        parinte->rotatie_stanga();
        if(parinte==ar.start)
            ar.start=parinte->p;
    }
    else
    {
        if(frate->fs!=nullptr)
            frate->fs->culoare=0;
        nod* parinte=n->p;
        parinte->rotatie_dreapta();
        if(parinte==ar.start)
            ar.start=parinte->p;
    }
}
void strg_c5(nod* n, arbori_rosu_negru &ar)
{
    nod* frate;
    if(n->p->fs==n)
        frate=n->p->fd;
    else
        frate=n->p->fs;
    if(frate->culoare==0)
    {
        if((n==n->p->fs)&&(frate->fd==nullptr||frate->fd->culoare==0)&&(frate->fs->culoare==1))
        {
            frate->culoare=1;
            frate->fs->culoare=0;
            frate->rotatie_dreapta();
            if(frate==ar.start)
                ar.start=frate->p;
        }
        else if((n==n->p->fd)&&(frate->fs==nullptr||frate->fs->culoare==0)&&(frate->fd->culoare==1))
        {
            frate->culoare=1;
            frate->fd->culoare=0;
            frate->rotatie_stanga();
            if(frate==ar.start)
                ar.start=frate->p;

        }
    }
    strg_c6(n,ar);
}
void strg_c4(nod* n, arbori_rosu_negru &ar)
{
    nod* frate;
    if(n->p->fs==n)
        frate=n->p->fd;
    else
        frate=n->p->fs;
    if((n->p!=nullptr&&n->p->culoare==1)&&(frate->culoare==0)&&(frate->fd==nullptr||frate->fd->culoare==0)&&(frate->fs==nullptr||frate->fs->culoare==0))
    {
        frate->culoare=1;
        n->p->culoare=0;
    }
    else
        strg_c5(n,ar);

}

void strg_c3(nod* n, arbori_rosu_negru &ar)
{
    nod* frate;
    if(n->p->fs==n)
        frate=n->p->fd;
    else
        frate=n->p->fs;
    if((n->p==nullptr||n->p->culoare==0)&&(frate->culoare==0)&&(frate->fd==nullptr||frate->fd->culoare==0)&&(frate->fs==nullptr||frate->fs->culoare==0))
    {
        frate->culoare=1;
        strg_c1(n->p,ar);
    }
    else
    {
        strg_c4(n,ar);
    }
}

void strg_c2(nod* n, arbori_rosu_negru &ar)
{
    nod* frate;
    if(n->p->fs==n)
        frate=n->p->fd;
    else
        frate=n->p->fs;
    if(frate!=nullptr)
        if(frate->culoare==1)
        {
            n->p->culoare=1;
            frate->culoare=0;
            if(n==n->p->fs)
            {
                nod* parinte=n->p;
                parinte->rotatie_stanga();
                if(parinte==ar.start)
                    ar.start=parinte->p;
            }
            else
            {
                nod* parinte=n->p;
                parinte->rotatie_dreapta();
                if(parinte==ar.start)
                    ar.start=parinte->p;
            }

        }
    strg_c3(n,ar);
}

void strg_c1(nod* n, arbori_rosu_negru &ar)
{
    if(n->p!=nullptr)
        strg_c2(n,ar);
}

void strg_un_fiu(nod*n,arbori_rosu_negru &ar)//n are cel mult un fiu nenul
{
    nod* fiu;
    if(n->fd==nullptr)
        fiu=n->fs;
    else
        fiu=n->fd;


    if((n->culoare==0)&&(fiu==nullptr))
    {
        strg_c1(n,ar);
        n->inlocuire(fiu);
        if(n==ar.start)
            ar.start=fiu;
    }

    else
    {
        n->inlocuire(fiu);
        if(n==ar.start)
            ar.start=fiu;
        if (n->culoare==0)
        {

            if(fiu->culoare==1)
                fiu->culoare=0;
            else
                strg_c1(fiu,ar);

        }


    }
    n->fs=nullptr;
    n->fd=nullptr;
    delete n;
}

int main()
{
    arbori_rosu_negru ar;
    int q;
    fin>>q;
    while(q)
    {
        int z;
        fin>>z;
        if(z==1)
        {
            long x;
            fin>>x;
            ar.inserare(x);
        }
        else if(z==2)
        {
            long x;
            fin>>x;
            ar.stergere(x);
        }
        else if(z==3)
        {
            long x;
            fin>>x;
            fout<<ar.find_val(x)<<"\n";

        }
        else if(z==4)
        {
            long x;
            fin>>x;
            fout<<ar.predecesor(x)<<"\n";
        }
        else if(z==5)
        {
            long x;
            fin>>x;
            fout<<ar.succesor(x)<<"\n";
        }
        else if(z==6)
        {
            long x,y;
            fin>>x>>y;
            ar.interval(x,y);
        }
        q--;
    }
    return 0;
}
