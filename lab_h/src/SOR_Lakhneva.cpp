#include <stdlib.h>

typedef void (*sort_func_t) (int array[], int size);

enum Bool {
	False,
	True
};

typedef enum {
	SORT_NA = -1,
	// Квадратичные
	SORT_SELECTION, SORT_INSERTION, SORT_BUBBLE,
	// Субквадратичные
	SORT_SHELL,
	// Квазилинейные
	SORT_COMB, SORT_MERGE, SORT_QUICK, SORT_HEAP,
	// Линейные
	SORT_COUNT, SORT_RADIX, SORT_BUCKET,
} sort_family_t;

typedef enum {
	SORT_QUADRATIC,
	SORT_SUBQUADRATIC,
	SORT_QUASILINEAR,
	SORT_LINEAR
} sort_complexity_t;

typedef struct {
	sort_func_t sort; // Функция сортировки
	sort_family_t family; // Базовое семейство
	sort_complexity_t complexity; // Вычислительная сложность
	char const* name; // Имя, описывающее вариацию
} sort_info_t;


// сортировка выбором со сдвигом (от макс к мин ) done
static void SelectionSortShift(int* a, int n) {
	int k = n;
	for (int j = 0; j < n; j++) {
		int min = a[0];
		int ind = 0;
		// Выбирается минимальный элемент в массиве и запоминается. 
		for (int i = 0; i < k; i++) {
			if (a[i] <= min) { // <= для стабильности
				ind = i;
				min = a[i];
			}
		}
		//Затем удаляется, а все последующие за ним элементы сдвигаются на один влево.
		for (ind; ind + 1 < k; ind++) {
			a[ind] = a[ind + 1];
		}
		//Сам элемент заносится на освободившуюся последнюю позицию.
		a[k - 1] = min;
		k--;
	}
}

// выбором с обменом (от макс к мин) done
static void SelectionSortExchange(int* a, int n) {
	int k = n;
	for (int i = 0; i < n - 1; i++) {
		//устанавливаем начальное значение минимального индекса
		int ind = 0;
		//находим индекс минимального элемента 
		for (int j = 0; j < k; j++) {
			if (a[j] < a[ind]) {
				ind = j;
			}
		}
		//меняем значения местами 
		int temp = a[k - 1];
		a[k - 1] = a[ind];
		a[ind] = temp;
		k--;
	}
}

//сортировка пузырьком 
//шаг сортировки состоит в проходе снизу вверх по массиву.
//По пути просматриваются пары соседних элементов.
//Если элементы некоторой пары находятся в неправильном порядке, то меняем их местами.
static void BubbleSort(int* a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (a[j] < a[j + 1]) {
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}

//сортировка пузырьком c остановкой на 1 раньше 
static void BubbleSort1(int* a, int n) {
	int k = n;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < k; j++) {
			if (a[j] < a[j + 1]) {
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
		k--;
	}
}

// остановка если не было обменов
static void BubbleSortFlagCheck(int* a, int n) {
	for (int i = 0; i < n; i++) {
		int flag = True;
		for (int j = 0; j < n - 1; j++) {
			if (a[j] < a[j + 1]) {
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				flag = False;
			}
		}
		if (flag) {
			break;
		}
	}
}

// шейкер-сортировка 
static void CocktailSort(int* a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[j] < a[j + 1]) {
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
		for (int j = n - 1; j >= 1; j--) {
			if (a[j] > a[j - 1]) {
				int temp = a[j];
				a[j] = a[j - 1];
				a[j - 1] = temp;
			}
		}
	}
}

// сортировка расческой с фактором уменьшения
static void CombSort(int* a, int n) {
	double reduction = 1.247;
	int distance = n - 1;

	while (distance >= 1) {
		for (int i = 0; i + distance < n; i++) {
			if (a[i] < a[i + distance]) {
				int temp = a[i];
				a[i] = a[i + distance];
				a[i + distance] = temp;
			}
		}
		distance /= reduction;
	}
}

__declspec(dllexport) sort_info_t* (GetSortList(int* count)) {
	static sort_info_t arr[] = { {BubbleSort, SORT_BUBBLE, SORT_QUADRATIC, "BASIC BUBBLE SORT"},
{SelectionSortShift, SORT_SELECTION, SORT_QUADRATIC, "SELECTION SORT W/ SHIFT"},
{SelectionSortExchange, SORT_SELECTION, SORT_QUADRATIC, "SELECTION SORT W/ EXCHANGE"},
{BubbleSort1, SORT_BUBBLE, SORT_QUADRATIC, "BUBBLE SORT EARLIER STOP"},
{BubbleSortFlagCheck, SORT_BUBBLE, SORT_QUADRATIC, "BUBBLE SORT STOP IF NO EXCHANGES"},
{CocktailSort, SORT_BUBBLE, SORT_QUADRATIC, "COCKTAIL SORT"},
{CombSort, SORT_COMB, SORT_QUASILINEAR, "COMB SORT"},
	};
	*count = sizeof(arr) / sizeof(arr[0]);
	return arr;
}
