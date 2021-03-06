#pragma once
#include <QList>
#include "Node.hh"
#include "Properties.hh"

class VisitorNode;
class EvalVisitor;

class Gate : public Node
{
protected:
	Properties prop;
	QList<Node*> children;
public:
	Gate(QString name,bool keep = true);
	virtual ~Gate();

	Properties&		getProperties();
	QList<Node*>&	getChildren();
	void 	balanceNodePos();	
	Node*	search(QPoint around);
	void	remove();
};

class And : public Gate
{
public:
	And(QString name,bool keep = true);
	~And();

	bool check(QList<QString>& errors);
	void accept(VisitorNode& visitor);
	double accept(EvalVisitor& eval);
};

class Inhibit : public Gate
{
	protected:
	bool condition;
	public:
	Inhibit(QString name,bool keep = true);
	~Inhibit();

	bool getCondition() const;
	void setCondition(bool condition);	
	bool check(QList<QString>& errors);
	void accept(VisitorNode& visitor);
	double accept(EvalVisitor& eval);
}; 

class Or : public Gate
{
public:
	Or(QString name,bool keep = true);
	~Or();

	bool check(QList<QString>& errors);
	void accept(VisitorNode& visitor);
	double accept(EvalVisitor& eval);

};

class VotingOR : public Gate
{
	protected:
	int k;
	Gate* subTree;
	public:
	VotingOR(QString name,bool keep = true);
	~VotingOR();

	void updateSubTree();
	Node* generateComb(int i,int k,int n);
	int getK() const;
	void setK(int k);
	Gate* getSubTree();
	bool check(QList<QString>& errors) override;
	void remove() override;
	void accept(VisitorNode& visitor) override;
	double accept(EvalVisitor& eval) override;
};

class Xor : public Gate
{
public:
	Xor(QString name,bool keep = true);
	~Xor();

	bool check(QList<QString>& errors);
	void accept(VisitorNode& visitor);
    double accept(EvalVisitor& eval);
};
