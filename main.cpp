#include <action_curve3d.h>
#include <iostream>
#include <string>

#include <fstream>

void dots3(SArray<MbCartPoint3D> *points)
{
	(*points).Add(MbCartPoint3D(0, 0, 0));
	(*points).Add(MbCartPoint3D(1, 2, 0));
	(*points).Add(MbCartPoint3D(2, 0, 0));
}

void dots7(SArray<MbCartPoint3D>* points)
{
	(*points).Add(MbCartPoint3D(0, 0, 0));
	(*points).Add(MbCartPoint3D(1, 2, 0));
	(*points).Add(MbCartPoint3D(2, 0, 0));
	(*points).Add(MbCartPoint3D(4, 2, 0));
	(*points).Add(MbCartPoint3D(6, 0, 0));
	(*points).Add(MbCartPoint3D(8, 4, 0));
	(*points).Add(MbCartPoint3D(9, 0, 0));

}

void CreateSplineCurve(MbCurve3D* *result, SArray<MbCartPoint3D> points, MbeSpaceType type)
{
	SplineCurve(points, false, type, *result);
}


/// <summary>
/// Алгоритм линеаризации. Если расстояние от части кривой до ломаной больше радиуса r, то эта часть кривой делится на два
/// </summary>
/// <param name="curve"> Кривая </param>
/// <param name="r"> Необходимое расстояние от кривой до ломаной</param>
/// <returns>Массив координат точек линеаризации</returns>
SArray<MbCartPoint3D> Linearization(MbCurve3D* curve, double r)
{
	SArray<MbCartPoint3D> result; //массив координат ломаной
	std::vector <double> paramForCoords; //вектор параметров для нахождения координат 
	
	bool flag = true; //если все раасстояния от кривой до линеаризации попадают под радиус.
	
	double tMin = curve->GetTMin(),
		tMax = curve->GetTMax();

	//Вносим начало и конец кривой
	MbCartPoint3D ptStart = curve->GetLimitPoint(1), 
		ptEnd = curve->GetLimitPoint(2);

	paramForCoords.push_back(tMin);
	paramForCoords.push_back(tMax);

	result.Add(ptStart);
	result.Add(ptEnd);


	while (flag)
	{
		int i = 0;
		for (i = 0; i < paramForCoords.size() - 1; i++)
		{
			double tMid = (paramForCoords[i] + paramForCoords[i + 1]) / 2;
			MbCartPoint3D ptCenter = curve->PointOn(tMid);

			MbCurve3D* segment;
			Segment(result[i], result[i + 1], segment);

			if (segment->DistanceToPoint(ptCenter) > r)
			{
				paramForCoords.insert(paramForCoords.begin() + i + 1, tMid);
				result.AddAt(ptCenter, i + 1);
		
				break;
			}
		}
		if (i == result.Count()-1) flag = false;
	}
	return result;
}



void RecurLinearize(MbCurve3D* curve, std::vector<double>& params, SArray<MbCartPoint3D>& points, int leftIndex, int rightIndex, double r)
{
	if (leftIndex >= rightIndex ) return;

	double tMid = (params[leftIndex] + params[rightIndex]) / 2;
	MbCartPoint3D ptCenter = curve->PointOn(tMid);

	MbCurve3D* segm;
	Segment(points[leftIndex], points[rightIndex], segm);
	double dist = segm->DistanceToPoint(ptCenter);
	delete segm;
	if (dist <= r) return;

	params.insert(params.begin() + rightIndex, tMid);
	points.InsertObj(points[rightIndex], ptCenter);

	int count = points.Count();
	RecurLinearize(curve, params, points, leftIndex, rightIndex, r);
	RecurLinearize(curve, params, points, rightIndex + (points.Count() - count), rightIndex + (points.Count() - count) + 1, r);

}
SArray<MbCartPoint3D> LinearizationRecurse(MbCurve3D* curve, double r)
{
	SArray<MbCartPoint3D> result; //массив координат ломаной
	std::vector <double> paramForCoords; //вектор параметров для нахождения координат 

	double tMin = curve->GetTMin(),
		tMax = curve->GetTMax();

	//Вносим начало и конец кривой
	MbCartPoint3D ptStart = curve->GetLimitPoint(1),
		ptEnd = curve->GetLimitPoint(2);

	
	paramForCoords.push_back(tMin);
	paramForCoords.push_back(tMax);

	result.Add(ptStart);
	result.Add(ptEnd);

	RecurLinearize(curve, paramForCoords, result, 0, result.Count() - 1, r);
	return result;
}





int main()
{
	SArray<MbCartPoint3D> points; //массив коорлинат для линеаризации
	MbCurve3D* curve; //кривая

	dots7(&points); //st_Bezier3D
	//dots3(&points); //st_Arc3D
	CreateSplineCurve(&curve, points, st_Bezier3D);


	points = LinearizationRecurse(curve, 0.1);


	for (int i = 0; i < points.Count(); i++)
	{
		std::cout << "" << points[i].x << " , " << points[i].y << "" << std::endl;
	}
	std::cout << "---------------------------" << std::endl;
	return 0;
}
