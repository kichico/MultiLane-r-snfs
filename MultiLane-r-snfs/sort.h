#pragma once
#ifndef SORT_H
#define SORT_H
#include <vector>

std::vector<int> _Create_number(int size) {
	std::vector<int> B(size);
	for (int i = 0; i < size; i++) B[i] = i;
	return B;
}

template<typename _T> 
_T _MAX(_T x, _T y)
{
	return x < y ? y : x;
}

struct MyStruct
{
	int tmin;
	int tmax;
};

MyStruct _search_min_max(std::vector<int> A) {
	int tmin = A[0];
	int tmax = A[0];
	for (int i = 1; i < A.size(); i++) {
		if (tmin > A[i]) tmin = A[i];
		if (tmax < A[i]) tmax = A[i];
	}
	MyStruct ms;
	ms.tmin = tmin;
	ms.tmax = tmax;
	return ms;
}

class QuickSort {
public:
	template<typename _T>
	std::vector<_T> ascendingsort_data(std::vector<_T> A)
	{
		int left = 0;
		int right = A.size() - 1;
		ascendingsort(&A, left, right);
		return A;
	}

	template<typename _T>
	std::vector<int> ascendingsort_number(std::vector<_T> A)
	{
		int left = 0;
		int right = A.size() - 1;
		std::vector<int> B = _Create_number(A.size());
		ascendingsort(&A, &B, left, right);
		return B;
	}

	template<typename _T>
	std::vector<_T> descendingsort_data(std::vector<_T> A)
	{
		int left = 0;
		int right = A.size() - 1;
		descendingsort(&A, left, right);
		return A;
	}

	template<typename _T>
	std::vector<int> descendingsort_number(std::vector<_T> A)
	{
		int left = 0;
		int right = A.size() - 1;
		std::vector<int> B = _Create_number(A.size());
		descendingsort(&A, &B, left, right);
		return B;
	}

private:
	template<typename _T>
	_T med3(_T x, _T y, _T z) { return _MAX(_MAX(x, y), z); }

	template<typename _T>
	void ascendingsort(std::vector<_T>* A, int left, int right) {
		if (left < right) {
			_T pivot = med3((*A)[left], (*A)[left + (right - left) / 2], (*A)[right]);
			int l = left;
			int r = right;
			while (true) {
				while ((*A)[l] < pivot) l++;
				while ((*A)[r] > pivot) r--;
				if (l >= r) break;
				_T t = (*A)[r];
				(*A)[r] = (*A)[l];
				(*A)[l] = t;
				l++;
				r--;
			}
			ascendingsort(A, left, l - 1);
			ascendingsort(A, r + 1, right);
		}
	}

	template<typename _T>
	void ascendingsort(std::vector<_T>* A, std::vector<int>* B, int left, int right)
	{
		if (left < right) {
			_T pivot = med3((*A)[left], (*A)[left + (right - left) / 2], (*A)[right]);
			int l = left;
			int r = right;
			while (true) {
				while ((*A)[l] < pivot) l++;
				while ((*A)[r] > pivot) r--;
				if (l >= r) break;
				_T tA = (*A)[r];
				(*A)[r] = (*A)[l];
				(*A)[l] = tA;
				int tB = (*B)[r];
				(*B)[r] = (*B)[l];
				(*B)[l] = tB;
				l++;
				r--;
			}
			ascendingsort(A, B, left, l - 1);
			ascendingsort(A, B, r + 1, right);
		}
	}

	template<typename _T>
	void descendingsort(std::vector<_T>* A, int left, int right)
	{
		if (left < right) {
			_T pivot = med3((*A)[left], (*A)[left + (right - left) / 2], (*A)[right]);
			int l = left;
			int r = right;
			while (true) {
				while ((*A)[l] > pivot) l++;
				while ((*A)[r] < pivot) r--;
				if (l >= r) break;
				_T t = (*A)[r];
				(*A)[r] = (*A)[l];
				(*A)[l] = t;
				l++;
				r--;
			}
			descendingsort(A, left, l - 1);
			descendingsort(A, r + 1, right);
		}
	}

	template<typename _T>
	void descendingsort(std::vector<_T>* A, std::vector<int>* B, int left, int right)
	{
		if (left < right) {
			_T pivot = med3((*A)[left], (*A)[left + (right - left) / 2], (*A)[right]);
			int l = left;
			int r = right;
			while (true) {
				while ((*A)[l] > pivot) l++;
				while ((*A)[r] < pivot) r--;
				if (l >= r) break;
				_T tA = (*A)[r];
				(*A)[r] = (*A)[l];
				(*A)[l] = tA;
				int tB = (*B)[r];
				(*B)[r] = (*B)[l];
				(*B)[l] = tB;
				l++;
				r--;
			}
			descendingsort(A, B, left, l - 1);
			descendingsort(A, B, r + 1, right);
		}
	}
};

class CountingSort {
public:
	std::vector<int> ascendingsort_data(std::vector<int> A)
	{
		std::vector<int> B(A.size());
		MyStruct ms = _search_min_max(A);
		std::vector<int> count = counting_ascending(A, ms);
		int n = 0;
		for (int i = 0; i < count.size(); i++) for (int j = 0; j < count[i]; j++) {
			B[n] = ms.tmin + i;
			n++;
		}
		return B;
	}

	template<typename _T>
	std::vector<_T> ascendingsort_data(std::vector<_T> A, std::vector<_T> Emergence_list)
	{
		QuickSort qc;
		Emergence_list = qc.ascendingsort_data(Emergence_list);
		std::vector<int> count = counting(A, Emergence_list);
		std::vector<_T> B(A.size());
		int n = 0;
		for (int i = 0; i < count.size(); i++) for (int j = 0; j < count[i]; j++) {
			B[n] = Emergence_list[i];
			n++;
		}
		return B;
	}

	std::vector<int> descendingsort_data(std::vector<int> A)
	{
		std::vector<int> B(A.size());
		MyStruct ms = _search_min_max(A);
		std::vector<int> count = counting_descending(A, ms);
		int n = 0;
		for (int i = 0; i < count.size(); i++) for (int j = 0; j < count[i]; j++) {
			B[n] = ms.tmax - i;
			n++;
		}
		return B;
	}

	template<typename _T>
	std::vector<_T> descendingsort_data(std::vector<_T> A, std::vector<_T> Emergence_list)
	{
		QuickSort qc;
		Emergence_list = qc.descendingsort_data(Emergence_list);
		std::vector<int> count = counting(A, Emergence_list);
		std::vector<_T> B(A.size());
		int n = 0;
		for (int i = 0; i < count.size(); i++) for (int j = 0; j < count[i]; j++) {
			B[n] = Emergence_list[i];
			n++;
		}
		return B;
	}

private:
	std::vector<int> counting_ascending(std::vector<int> A, MyStruct ms)
	{
		std::vector<int> count(ms.tmax - ms.tmin + 1, 0);
		for (int i = 0; i < A.size(); i++) {
			int n = A[i] - ms.tmin;
			count[n]++;
		}
		return count;
	}

	std::vector<int> counting_descending(std::vector<int> A, MyStruct ms)
	{
		std::vector<int> count(ms.tmax - ms.tmin + 1, 0);
		for (int i = 0; i < A.size(); i++) {
			int n = ms.tmax - A[i];
			count[n]++;
		}
		return count;
	}

	template<typename _T>
	std::vector<int> counting(std::vector<_T> A, std::vector<_T> B)
	{
		std::vector<int> count(B.size(), 0);
		for (int i = 0; i < A.size(); i++) for (int j = 0; j < B.size(); j++) if (A[i] == B[j]) {
			count[j]++;
			break;
		}
		return count;
	}

};

class MergeSort {
public:
	template<typename _T>
	std::vector<_T> ascendingsort_data(std::vector<_T> A)
	{
		ascendingsort(&A, 2);
		return A;
	}

	template<typename _T>
	std::vector<int> ascendingsort_number(std::vector<_T> A)
	{
		std::vector<int> B = _Create_number(A.size());
		ascendingsort(&A, &B, 2);
		return B;
	}

	template<typename _T>
	std::vector<_T> descendingsort_data(std::vector<_T> A)
	{
		descendingsort(&A, 2);
		return A;
	}

	template<typename _T>
	std::vector<int> descendingsort_number(std::vector<_T> A)
	{
		std::vector<int> B = _Create_number(A.size());
		descendingsort(&A, &B, 2);
		return B;
	}

private:
	template<typename _T>
	void ascendingsort(std::vector<_T>* A, int n)
	{
		if (n < (*A).size()) {
			int c = 0;
			std::vector<_T> b((*A).size());
			while (c < (*A).size()) {
				int l = c;
				int m = l + n / 2;
				if (m > (*A).size()) m = (*A).size();
				int r = m;
				int f = l + n;
				if (f > (*A).size()) f = (*A).size();
				int cc = 0;
				if (m < f) {
					while (cc < n) {
						if ((*A)[l] < (*A)[r]) {
							b[c] = (*A)[l];
							l++;
							c++;
							cc++;
						}
						else if ((*A)[r] < (*A)[l]) {
							b[c] = (*A)[r];
							r++;
							c++;
							cc++;
						}
						else {
							b[c] = (*A)[l];
							b[c + 1] = (*A)[r];
							c += 2;
							cc += 2;
							l++;
							r++;
						}
						if (l == m || r == f || c == (*A).size()) break;
					}
					if (cc < n) {
						while (l < m) {
							b[c] = (*A)[l];
							c++;
							l++;
						}
						while (r < f) {
							b[c] = (*A)[r];
							c++;
							r++;
						}
					}
				}
				else {
					while (l < m) {
						b[c] = (*A)[l];
						c++;
						l++;
					}
				}
			}
			n *= 2;
			(*A) = b;
			ascendingsort(A, n);
		}
		n /= 2;
		int l = 0;
		int m = n;
		int r = m;
		int f = (*A).size();
		int c = 0;
		std::vector<_T> b((*A).size());
		while (c < (*A).size()) {
			if ((*A)[l] < (*A)[r]) {
				b[c] = (*A)[l];
				l++;
				c++;
			}
			else if ((*A)[r] < (*A)[l]) {
				b[c] = (*A)[r];
				r++;
				c++;
			}
			else {
				b[c] = (*A)[l];
				b[c + 1] = (*A)[r];
				c += 2;
				l++;
				r++;
			}
			if (l == m || r == f) break;
		}
		while (l < m) {
			b[c] = (*A)[l];
			l++;
			c++;
		}
		while (r < f) {
			b[c] = (*A)[r];
			r++;
			c++;
		}
		(*A) = b;
	}

	template<typename _T>
	void ascendingsort(std::vector<_T>* A, std::vector<int>* B, int n)
	{
		if (n < (*A).size()) {
			int c = 0;
			std::vector<_T> ba((*A).size());
			std::vector<int> bb((*B).size());
			while (c < (*A).size()) {
				int l = c;
				int m = l + n / 2;
				if (m > (*A).size()) m = (*A).size();
				int r = m;
				int f = l + n;
				if (f > (*A).size()) f = (*A).size();
				int cc = 0;
				if (m < f) {
					while (cc < n) {
						if ((*A)[l] < (*A)[r]) {
							ba[c] = (*A)[l];
							bb[c] = (*B)[l];
							l++;
							c++;
							cc++;
						}
						else if ((*A)[r] < (*A)[l]) {
							ba[c] = (*A)[r];
							bb[c] = (*B)[r];
							r++;
							c++;
							cc++;
						}
						else {
							ba[c] = (*A)[l];
							ba[c + 1] = (*A)[r];
							bb[c] = (*B)[l];
							bb[c + 1] = (*B)[r];
							c += 2;
							cc += 2;
							l++;
							r++;
						}
						if (l == m || r == f || c == (*A).size()) break;
					}
					if (cc < n) {
						while (l < m) {
							ba[c] = (*A)[l];
							bb[c] = (*B)[l];
							c++;
							l++;
						}
						while (r < f) {
							ba[c] = (*A)[r];
							bb[c] = (*B)[r];
							c++;
							r++;
						}
					}
				}
				else {
					while (l < m) {
						ba[c] = (*A)[l];
						bb[c] = (*B)[r];
						c++;
						l++;
					}
				}
			}
			n *= 2;
			(*A) = ba;
			(*B) = bb;
			ascendingsort(A, B, n);
		}
		n /= 2;
		int l = 0;
		int m = n;
		int r = m;
		int f = (*A).size();
		int c = 0;
		std::vector<_T> ba((*A).size());
		std::vector<_T> bb((*B).size());
		while (c < (*A).size()) {
			if ((*A)[l] < (*A)[r]) {
				ba[c] = (*A)[l];
				bb[c] = (*B)[l];
				l++;
				c++;
			}
			else if ((*A)[r] < (*A)[l]) {
				ba[c] = (*A)[r];
				bb[c] = (*B)[r];
				r++;
				c++;
			}
			else {
				ba[c] = (*A)[l];
				ba[c + 1] = (*A)[r];
				bb[c] = (*B)[l];
				bb[c + 1] = (*B)[r];
				c += 2;
				l++;
				r++;
			}
			if (l == m || r == f) break;
		}
		while (l < m) {
			ba[c] = (*A)[l];
			bb[c] = (*B)[l];
			l++;
			c++;
		}
		while (r < f) {
			ba[c] = (*A)[r];
			bb[c] = (*B)[r];
			r++;
			c++;
		}
		(*A) = ba;
		(*B) = bb;
	}

	template<typename _T>
	void descendingsort(std::vector<_T>* A, int n)
	{
		if (n < (*A).size()) {
			int c = 0;
			std::vector<_T> b((*A).size());
			while (c < (*A).size()) {
				int l = c;
				int m = l + n / 2;
				if (m > (*A).size()) m = (*A).size();
				int r = m;
				int f = l + n;
				if (f > (*A).size()) f = (*A).size();
				int cc = 0;
				if (m < f) {
					while (cc < n) {
						if ((*A)[l] > (*A)[r]) {
							b[c] = (*A)[l];
							l++;
							c++;
							cc++;
						}
						else if ((*A)[r] > (*A)[l]) {
							b[c] = (*A)[r];
							r++;
							c++;
							cc++;
						}
						else {
							b[c] = (*A)[l];
							b[c + 1] = (*A)[r];
							c += 2;
							cc += 2;
							l++;
							r++;
						}
						if (l == m || r == f || c == (*A).size()) break;
					}
					if (cc < n) {
						while (l < m) {
							b[c] = (*A)[l];
							c++;
							l++;
						}
						while (r < f) {
							b[c] = (*A)[r];
							c++;
							r++;
						}
					}
				}
				else {
					while (l < m) {
						b[c] = (*A)[l];
						c++;
						l++;
					}
				}
			}
			n *= 2;
			(*A) = b;
			descendingsort(A, n);
		}
		n /= 2;
		int l = 0;
		int m = n;
		int r = m;
		int f = (*A).size();
		int c = 0;
		std::vector<_T> b((*A).size());
		while (c < (*A).size()) {
			if ((*A)[l] > (*A)[r]) {
				b[c] = (*A)[l];
				l++;
				c++;
			}
			else if ((*A)[r] > (*A)[l]) {
				b[c] = (*A)[r];
				r++;
				c++;
			}
			else {
				b[c] = (*A)[l];
				b[c + 1] = (*A)[r];
				c += 2;
				l++;
				r++;
			}
			if (l == m || r == f) break;
		}
		while (l < m) {
			b[c] = (*A)[l];
			l++;
			c++;
		}
		while (r < f) {
			b[c] = (*A)[r];
			r++;
			c++;
		}
		(*A) = b;
	}

	template<typename _T>
	void descendingsort(std::vector<_T>* A, std::vector<int>* B, int n)
	{
		if (n < (*A).size()) {
			int c = 0;
			std::vector<_T> ba((*A).size());
			std::vector<int> bb((*B).size());
			while (c < (*A).size()) {
				int l = c;
				int m = l + n / 2;
				if (m > (*A).size()) m = (*A).size();
				int r = m;
				int f = l + n;
				if (f > (*A).size()) f = (*A).size();
				int cc = 0;
				if (m < f) {
					while (cc < n) {
						if ((*A)[l] > (*A)[r]) {
							ba[c] = (*A)[l];
							bb[c] = (*B)[l];
							l++;
							c++;
							cc++;
						}
						else if ((*A)[r] > (*A)[l]) {
							ba[c] = (*A)[r];
							bb[c] = (*B)[r];
							r++;
							c++;
							cc++;
						}
						else {
							ba[c] = (*A)[l];
							ba[c + 1] = (*A)[r];
							bb[c] = (*B)[l];
							bb[c + 1] = (*B)[r];
							c += 2;
							cc += 2;
							l++;
							r++;
						}
						if (l == m || r == f || c == (*A).size()) break;
					}
					if (cc < n) {
						while (l < m) {
							ba[c] = (*A)[l];
							bb[c] = (*B)[l];
							c++;
							l++;
						}
						while (r < f) {
							ba[c] = (*A)[r];
							bb[c] = (*B)[r];
							c++;
							r++;
						}
					}
				}
				else {
					while (l < m) {
						ba[c] = (*A)[l];
						bb[c] = (*B)[r];
						c++;
						l++;
					}
				}
			}
			n *= 2;
			(*A) = ba;
			(*B) = bb;
			descendingsort(A, B, n);
		}
		n /= 2;
		int l = 0;
		int m = n;
		int r = m;
		int f = (*A).size();
		int c = 0;
		std::vector<_T> ba((*A).size());
		std::vector<_T> bb((*B).size());
		while (c < (*A).size()) {
			if ((*A)[l] > (*A)[r]) {
				ba[c] = (*A)[l];
				bb[c] = (*B)[l];
				l++;
				c++;
			}
			else if ((*A)[r] > (*A)[l]) {
				ba[c] = (*A)[r];
				bb[c] = (*B)[r];
				r++;
				c++;
			}
			else {
				ba[c] = (*A)[l];
				ba[c + 1] = (*A)[r];
				bb[c] = (*B)[l];
				bb[c + 1] = (*B)[r];
				c += 2;
				l++;
				r++;
			}
			if (l == m || r == f) break;
		}
		while (l < m) {
			ba[c] = (*A)[l];
			bb[c] = (*B)[l];
			l++;
			c++;
		}
		while (r < f) {
			ba[c] = (*A)[r];
			bb[c] = (*B)[r];
			r++;
			c++;
		}
		(*A) = ba;
		(*B) = bb;
	}

};

class Sort {
public:
	QuickSort quicksort;
	CountingSort countingsort;
	MergeSort mergesort;
};

#endif //SORT_H
