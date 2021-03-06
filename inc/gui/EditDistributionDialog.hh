#pragma once
#include "Dialog.hh"

// Le visiteur (pour automatiquement choisir le bon dialogue)
class VisitorEditDistribution : public VisitorDistribution
{
private:
	QWidget *parent;
	Editor &editor;

public:
	VisitorEditDistribution(QWidget *parent, Editor &editor);

	void visit(Constant &distribution);
	void visit(Exponential &distribution);
	void visit(Weibull &distribution);
};

class EditConstantDialog : public PropertiesDialog
{
	Q_OBJECT

private:
	Editor &editor;
	Constant &dist;
	DoubleSpinBox *value;

public:
	EditConstantDialog(QWidget *parent, Editor &editor, Constant &dist);
};


class EditExponentialDialog : public PropertiesDialog
{
	Q_OBJECT

private:
	Editor &editor;
	Exponential &dist;
	DoubleSpinBox *lambda;

public:
	EditExponentialDialog(QWidget *parent, Editor &editor, Exponential &dist);
};

class EditWeibullDialog : public PropertiesDialog
{
	Q_OBJECT

private:
	Editor &editor;
	Weibull &dist;
	DoubleSpinBox *scale;
	DoubleSpinBox *shape;

public:
	EditWeibullDialog(QWidget *parent, Editor &editor, Weibull &dist);
};

