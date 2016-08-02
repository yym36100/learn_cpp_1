#include <stdio.h>

#define dgb_print(x) printf(x)

class shape {
public:
	int x, y;
	class shape *n;

	shape(int xx, int yy) : x(xx),y(yy),n(0)	{ dgb_print(" dbg: shape ctor\n"); }
	virtual ~shape()							{ dgb_print(" dbg: shape dtor\n"); }
	virtual void Draw()							{ PrintName();PrintPos();PrintSpecifics(); }
	virtual void Move(int xo, int yo)			{ x+=xo;y+=yo; }
	virtual void Scale(int sf)					{}

protected:
	void PrintPos()								{ printf(": x=%d, y=%d",x,y); }

private:
	virtual void PrintName() = 0;
	virtual void PrintSpecifics() = 0;
};

class ellipse : public shape {
public:
	int r1,r2;

	ellipse(int xx, int yy, int rr1=10, int rr2=20)
		:shape(xx,yy),r1(rr1),r2(rr2)			{ dgb_print(" dbg: ellipse ctor\n"); }
	~ellipse()									{ dgb_print(" dbg: ellipse dtor\n"); }

	virtual void Scale(int sf)					{ r1*=sf; r2*=sf;}

private:
	virtual void PrintName()					{ printf("ellipse"); }
	virtual void PrintSpecifics()				{ printf(" r1=%d, r2=%d\n",r1,r2); }
};

class circle : public ellipse{
public:
	circle(int xx, int yy, int rr1=10)
		:ellipse(xx,yy,rr1,rr1)					{ dgb_print(" dbg: circle ctor\n"); }
	~circle()									{ dgb_print(" dbg: circle dtor\n"); }

	virtual void Move(int xo, int yo)			{ ellipse::Move(xo,yo);x+=xo; printf("!! tricky moving of circle (don't ask why)\n"); }

private:
	virtual void PrintName()					{ printf("circle"); }
	virtual void PrintSpecifics()				{ printf(" r=%d\n",r1); }
};

class rectangle : public shape{
public:
	int w,h;

	rectangle(int xx, int yy, int ww=10, int hh=20)
		:shape(xx,yy),w(ww),h(hh)				{ dgb_print(" dbg: rectangle ctor\n"); }
	~rectangle()								{ dgb_print(" dbg: rectangle dtor\n"); }

	virtual void Scale(int sf)					{ w*=sf; h*=sf;}

private:
	virtual void PrintName()					{ printf("rectangle"); }
	virtual void PrintSpecifics()				{ printf(" w=%d, h=%d\n",w,h); }
};

class square : public rectangle{
public:
	square(int xx, int yy, int size=10)
		:rectangle(xx,yy,size,size)				{ dgb_print(" dbg: square ctor\n"); }
	~square()									{ dgb_print(" dbg: square dtor\n"); }

	virtual void Scale(int sf)					{printf("!! square is not scalable (don't ask why..)\n");}

private:
	virtual void PrintName()					{ printf("square"); }
	virtual void PrintSpecifics()				{ printf(" size=%d\n",w); }
};

class triangle : public shape{
public:
	int size;
	triangle(int xx, int yy, int sz=10)
		:shape(xx,yy),size(sz)					{ dgb_print(" dbg: triangle ctor\n"); }
	~triangle()									{ dgb_print(" dbg: triangle dtor\n"); }

	virtual void Scale(int sf)					{ printf("!! triangle not scalable\n");}
	virtual void Move(int xo, int yo)			{ printf("!! triangle not moveable\n");}
	virtual void Draw()							{ printf("completely different triangle drawing "); PrintPos(); printf(" no specifics\n");};

private:
	virtual void PrintName()					{ printf("triangle"); }
	virtual void PrintSpecifics()				{ printf(" size=%d\n",size); }
};


class container{
public:
	shape *s;
	typedef void (container::*fp)(class shape *p);

	container():s(0)							{}
	~container(){
		shape *t = s,*t1;
		while(t){ t1 = t; t = t->n; delete t1; }
	}

	void Add( shape *p){
		shape *t = s;
		if(!t) { s= p; return;}
		while(t->n) t=t->n; t->n = p;
	}

	void for_each(fp p){
		shape *t = s;	
		while(t){ (this->*p)(t); t=t->n; }
	}

	void DrawAll()								{					for_each(&container::draw); }
	void MoveAll(int xo, int yo)				{ tmpx=xo;tmpy=yo;	for_each(&container::move); }
	void ScaleAll(int scale)					{ tmpscale = scale;	for_each(&container::scale); }

private:
	int tmpx,tmpy, tmpscale;

	void draw(shape *p)							{ p->Draw(); }
	void move(shape *p)							{ p->Move(tmpx,tmpy); }
	void scale(shape *p)						{ p->Scale(tmpscale); }	
};


int main(int argc, char* argv[])
{
	printf("Shape container created\n");
	container *c = new container();

	printf("\n>>>>>>>>Adding elements\n");
	c->Add(new ellipse(1,2));
	c->Add(new circle(3,4));
	c->Add(new rectangle(3,4));
	c->Add(new square(3,4));
	c->Add(new triangle(3,4));

	printf("\n----------------\n");
	c->DrawAll();

	printf("\n>>>>>>>>moving elements by (1,2)\n");
	c->MoveAll(1,2);
	printf("\n----------------\n");
	c->DrawAll();
	
	printf("\n>>>>>>>>scaling elements by 3\n");
	c->ScaleAll(3);
	printf("\n----------------\n");
	c->DrawAll();

	printf("\n>>>>>>>>destroying elements\n");
	delete c;

	return 0;
}

// TODO
// imagine that instead of simple printfs the methods do some really cool, next gen, bleeding edge stuff, but for now they are just printfs.
// restrictions: elements have to be allocated dynamically at the start (nr items and type is not known lets say ~100 of each type) and memory has to be freed at the end
// program should be able to handle new shape types e.g. pentagon without changing existing 'classes'
// challenge: transcribe program in C in such a way that is more simple and easy to understand, since C++ is broken and sucks anyway.. :)



