
#include <math.h>
#include "ElasticBodySimulator.h" 
#include <stdio.h>

using namespace Pirogronian;

ElasticBodySimulator::ElasticBodySimulator(int x, int y, int z, double granMass, double linkMass, double couple, double space, double initspace, double time) {
  
  _couple = couple;
  _timeSlice = time;
  _granMass = granMass;
  _linkMass = linkMass;
  _eqspace = space;
  
  _xSize = x;
  _ySize = y;
  _zSize = z;
  
//  printf("_gMatrix.size: %d\n", _gMatrix.size());
  
  for (int i = 0; i < x; i++) {
	 _gMatrix.append(QVector<QVector<Granule> >());
    for (int j = 0; j < y; j++) {
		_gMatrix[i].append(QVector<Granule>(z));
      for (int k = 0; k < z; k++) {
//			printf("%d, %d, %d, _gMatrix.size: %d\n", i, j, k, _gMatrix.size());
			ElasticBodySimulator::Granule g;
			g.speed.setX(0);
			g.speed.setY(0);
			g.speed.setZ(0);
			g.position.setX(initspace * i);
			g.position.setY(initspace * j);
			g.position.setZ(initspace * k);
			_gMatrix[i][j][k] = g;
      }
	 }
  }
}

ElasticBodySimulator::~ElasticBodySimulator() { }

QVector3D ElasticBodySimulator::forces(ElasticBodySimulator::Granule a, ElasticBodySimulator::Granule b) {
      QVector3D dis, fp;
    
      dis = distances(a, b);
    
      qreal f = _couple * (dis.length() - _eqspace);
    
      fp.setX(f * dis.x());
      fp.setY(f * dis.y());
      fp.setZ(f * dis.z());
    
      return fp;
    }

void ElasticBodySimulator::updateForce(int i, int j, int k) {
	ElasticBodySimulator::Granule &g =  _gMatrix[i][j][k];
	
	//printf("_gMatrix.size: %d\n", _gMatrix.size());
	
//		printf("_gMatrix.size: %d\n", _gMatrix.size());
//		printf("_gMatrix[%d].size: %d\n", i+1, _gMatrix[i+1].size());
//		printf("_gMatrix[%d][%d].size: %d\n", i+1, j, _gMatrix[i+1][j].size());
//		printf("_gMatrix[%d][%d][%d] mass:%f\n", i+1, j, k, _gMatrix[i+1][j][k].mass);
//		printf("_lMatrix[%d][%d][%d].mass:%f\n", i, j, k, _lMatrix[i][j][k].mass);
	g.force.setX(0);
	g.force.setY(0);
	g.force.setZ(0);
	
	if (i < _xSize - 1) g.force += forces(g, _gMatrix[i+1][j][k]);
	if (j < _ySize - 1) g.force += forces(g, _gMatrix[i][j+1][k]);
	if (k < _zSize - 1) g.force += forces(g, _gMatrix[i][j][k+1]);
	
	if (i > 0) {
		g.force += forces(g, _gMatrix[i-1][j][k]);
	}
	if (j > 0) {
		g.force += forces(g, _gMatrix[i][j-1][k]);
	}
	if (k > 0) {
		g.force += forces(g, _gMatrix[i][j][k-1]);
	}
	
	printf("Granule[%d][%d][%d]: f(%f, %f, %f)\n", i, j, k, g.force.x(), g.force.y(), g.force.z());
}

void ElasticBodySimulator::updatePosition(ElasticBodySimulator::Granule &g) {
	QVector3D accel, newspeed, movement;
	
	accel = g.force / granMass();
	
	newspeed = g.speed + accel * _timeSlice;
	
	movement = (g.speed + newspeed) / 2 * _timeSlice;
	
	g.position += movement;
	g.speed = newspeed;
}

void ElasticBodySimulator::doStep() {
	for (int i = 0; i < _xSize; i++)
		for (int j = 0; j < _ySize; j++)
			for (int k = 0; k < _zSize; k++)
				updateForce(i, j, k);
	
	//correctForces();
			
	for (int i = 0; i < _xSize; i++)
		for (int j = 0; j < _ySize; j++)
			for (int k = 0; k < _zSize; k++)
				updatePosition(_gMatrix[i][j][k]);
}

qreal ElasticBodySimulator::calcForces() {
	qreal sum = 0;
	
	for (int i = 0; i < _xSize; i++)
		for (int j = 0; j < _ySize; j++)
			for (int k = 0; k < _zSize; k++)
				sum += _gMatrix[i][j][k].force.length();
	
	return sum;
}

void ElasticBodySimulator::sumForces() {
	_totForce = calcForces();
}

void ElasticBodySimulator::correctForces() {
	qreal a = (calcForces() - _totForce) / (_xSize * _ySize * _zSize);
	qreal l, lp, cosx, cosy, cosz;
	
	for (int i = 0; i < _xSize; i++)
		for (int j = 0; j < _ySize; j++)
			for (int k = 0; k < _zSize; k++) {
				QVector3D f = _gMatrix[i][j][k].force;
				
				lp = f.length() - a;
				
				cosx = f.x() / f.length();
				cosy = f.y() / f.length();
				cosz = f.z() / f.length();
				
				f.setX(cosx * lp);
				f.setY(cosy * lp);
				f.setZ(cosz * lp);
				
				_gMatrix[i][j][k].force = f;
			}
			
}
