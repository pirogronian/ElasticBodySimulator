#ifndef __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__NEW_SIMULATION_DIALOG__H__ 
#define __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__NEW_SIMULATION_DIALOG__H__

#include <QtWidgets/QtWidgets>

namespace Pirogronian {
	
	class NewSimulationDialog : public QDialog {
		Q_OBJECT
		
		QSpinBox *_x, *_y, *_z;
		
		QDoubleSpinBox *_gMass, *_lMass, *_couple, *_space, *_initspace, *_time;
		
		QDialogButtonBox *_buttons;
		
		QVBoxLayout *_mLay;
		QFormLayout *_formLay;
		
	public:
		NewSimulationDialog(QWidget *p = 0);
		
		int x() { return _x->value(); }
		int y() { return _y->value(); }
		int z() { return _z->value(); }
		double granMass() { return _gMass->value(); }
		double linkMass() { return _lMass->value(); }
		double couple() { return _couple->value(); }
		double space() { return _space->value(); }
		double initspace() { return _initspace->value(); }
		double time() { return _time->value(); }
		
	};
	
};

#endif
