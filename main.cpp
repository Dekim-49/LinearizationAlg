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

//
//void CreateCurve()
//{
//	SArray<MbCartPoint3D> points;
//	MbCurve3D* result;
//
//	// Дуга окружности
//	/*dots3(&points);
//	CreateSplineCurve(&result, points, st_Arc3D);*/
//
//	
//	dots7(&points);
//	CreateSplineCurve(&result, points, st_Bezier3D);
//	
//
//	std::string filePath = "C:\\Users\\ASUS\\Desktop\\example.txt";
//	std::ofstream outFile(filePath);
//	if (!outFile.is_open()) {
//		std::cerr << "Ошибка открытия файла." << std::endl;
//	}
//
//	double len = result->GetParamLength();
//	double t = 0.0;
//	for (t = 0.0; t <= len; t += 0.1)
//	{
//		MbCartPoint3D pt;
//		result->PointOn(t, pt);
//
//		if (std::abs(pt.x) < std::exp(-16))
//		{
//			pt.x = 0.0;
//		}
//		if (std::abs(pt.y) < std::exp(-16))
//		{
//			pt.y = 0.0;
//		}
//		if (std::abs(pt.z) < std::exp(-16))
//		{
//			pt.z = 0.0;
//		}
//		//std::cout << "t = " << t << " pt = " << pt.x << " " << pt.y << " " << pt.z << std::endl;
//		outFile <<  "" << pt.x << "; " << pt.y << "" << std::endl;
//	}
//
//
//	//std::cout << "GetParamLength = " << result->GetParamLength() << "GetMetricLength = " << result->GetMetricLength();
//
//}

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
		for (i = 0; i < paramForCoords.Count()-1; i++)
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

int main()
{
	SArray<MbCartPoint3D> points; //массив коорлинат для линеаризации
	MbCurve3D* curve; //кривая

	dots7(&points);
	CreateSplineCurve(&curve, points, st_Bezier3D);


	points = Linearization(curve, 0.1);
	for (int i = 0; i < points.Count(); i++)
	{
		std::cout << "(" << points[i].x << " ; " << points[i].y << " ; " << points[i].z << std::endl;
	}
	return 0;
}
