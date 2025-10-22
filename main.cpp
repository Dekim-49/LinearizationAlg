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
/// Находит точку середины между двумя точками
/// </summary>
/// <param name="start">Точка начала</param>
/// <param name="end">Точка конца</param>
/// <returns>Точка центра</returns>
MbCartPoint3D FindCenter(MbCartPoint3D start, MbCartPoint3D end)
{
	MbCartPoint3D result;
	result.x = (start.x + end.x) / 2;
	result.y = (start.y + end.y) / 2;
	result.z = (start.z + end.z) / 2;
	return result;
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
	
	double len = curve->GetParamLength(); //длина кривой
	bool flag = true; //если все раасстояния от кривой до линеаризации попадают под радиус.
	
	double t = 0.0;

	//Вносим начало и конец кривой
	MbCartPoint3D ptStart, ptEnd;
	paramForCoords.push_back(t);
	paramForCoords.push_back(len);
	curve->PointOn(t, ptStart);
	curve->PointOn(len, ptEnd);
	result.Add(ptStart);
	result.Add(ptEnd);


	while (flag)
	{
		int i = 0;
		for (i = 0; i < result.Count()-1; i++)
		{
			if (curve->DistanceToPoint(FindCenter(result[i], result[i+1])) > r)
			{
				double n = (paramForCoords[i] + paramForCoords[i + 1]) / 2;
				auto iter = paramForCoords.cbegin();
				paramForCoords.emplace(iter + i + 1, n);

				MbCartPoint3D pt;
				curve->PointOn(paramForCoords[i + 1], pt);
				result.AddAt(pt, i + 1);
		
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
