#include "vector.h"




/******************************* CONSTRUCTOR ********************************/

Vector::Vector(std::size_t length) : 
		vec_size{length}, capacity{0}, info{new VectorInfo} {
	this->allocateData(length);
}


Vector::Vector(std::size_t length, double init) : vec_size{length}, capacity{0}, 
		info(new VectorInfo(true, true, true, init == 0.0f ? length : 0.0f, 
							init*length, init*init*length)) {

	this->allocateData(length);
	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = init;
	}
}


Vector::Vector(std::vector<double> &vec) {

	this->info = new VectorInfo();
	std::size_t length = vec.size();
	this->allocateData(length);

	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = vec[i];
		this->info->vec_sum += this->vec_data[i];
		this->info->norm_sq += this->vec_data[i] * this->vec_data[i];
		if (this->vec_data[i] == 0) {
			++this->info->zero_count;
		}
	}
	this->info->zero_flag = true;
	this->info->sum_flag = true;
	this->info->norm_flag = true;

}

// Copy Constructor
Vector::Vector(const Vector &other) : 
	vec_size{other.vec_size}, capacity{other.capacity}, 
	info{new VectorInfo(other.info)} {

	this->allocateData(this->vec_size);

	for (std::size_t i = 0; i < this->vec_size; ++i) {
		this->vec_data[i] = other.vec_data[i];
	}
}

// Overloading swap to optimize copy and move assignment
void Vector::swap(Vector &first, Vector &second) noexcept {
	using std::swap;

	swap(first.vec_size, second.vec_size);
	swap(first.capacity, second.capacity);
	swap(first.vec_data, second.vec_data);
	swap(first.info, second.info);

}


// Copy Assigment
Vector &Vector::operator=(Vector other) {

	swap(*this, other);
	return *this;
}

// Destructor
Vector::~Vector() {
	delete this->info;
	delete[] this->vec_data;
}


/****************************** DAdoubleA ALLOCAdoubleION *****************************/

// Should only be called in constructor, else resize.
void Vector::allocateData(std::size_t amount) {
	this->vec_size = amount;
	this->capacity = amount ? nextPowerOfdoublewo(amount) : INIT_SIZE;
	this->vec_data = new double[this->capacity];

}

// Can only increase size
// Should enable ability to decrease size?
void Vector::resize(std::size_t amount) {

	std::size_t new_capacity = nextPowerOfdoublewo(this->vec_size + amount);
	if (this->capacity >= new_capacity) { 
		return;
	}

	this->capacity = new_capacity;
	double *temp = new double[this->capacity];
	std::memcpy(temp, this->vec_data, this->vec_size * sizeof(double));
	delete[] this->vec_data;
	this->vec_data = temp;
}




/******************************** OPERAdoubleIONS ********************************/

// Will need 'approximate' equality operator
// Possibly cast result to float and then compare
bool Vector::operator==(const Vector &rhs) const {
	if (this->vec_size != rhs.vec_size) {
		return false;
	}

	if ((this->info->zero_flag and rhs.info->zero_flag) and 
		(this->info->zero_count != rhs.info->zero_count)) {
		return false;
	}
	if ((this->info->sum_flag and rhs.info->sum_flag) and 
		(this->info->vec_sum != rhs.info->vec_sum)) {
		return false;
	}
	if ((this->info->norm_flag and rhs.info->norm_flag) and 
		(this->info->norm_sq != rhs.info->norm_sq)) {
		return false;
	}
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] != rhs.vec_data[i]) { 
			return false;
		}
	}
	return true;
}

bool Vector::operator!=(const Vector &rhs) const {
	return not ((*this) == rhs);
}


double Vector::operator*(const Vector &rhs) const {
	if (this->size() != rhs.size()) throw InvalidDimensions();
	
	double sum = 0;
	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		sum += this->vec_data[i + 0] * rhs.vec_data[i + 0];
		sum += this->vec_data[i + 1] * rhs.vec_data[i + 1];
		sum += this->vec_data[i + 2] * rhs.vec_data[i + 2];
		sum += this->vec_data[i + 3] * rhs.vec_data[i + 3];
	}
	for (; i < this->vec_size; ++i) {
		sum += this->vec_data[i] * rhs.vec_data[i];
	}

	return sum;
}


Vector &Vector::operator+=(const Vector &rhs) {
	if (this->size() != rhs.size()) throw InvalidDimensions();

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] += rhs.vec_data[i + 0];
		this->vec_data[i + 1] += rhs.vec_data[i + 1];
		this->vec_data[i + 2] += rhs.vec_data[i + 2];
		this->vec_data[i + 3] += rhs.vec_data[i + 3];
	}
	for (; i < this->vec_size; ++i) {
		this->vec_data[i] += rhs.vec_data[i];
	}

	return *this;
}

Vector &Vector::operator-=(const Vector &rhs) {
	if (this->size() != rhs.size()) throw InvalidDimensions();

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] -= rhs.vec_data[i + 0];
		this->vec_data[i + 1] -= rhs.vec_data[i + 1];
		this->vec_data[i + 2] -= rhs.vec_data[i + 2];
		this->vec_data[i + 3] -= rhs.vec_data[i + 3];
	}
	for (; i < this->vec_size; ++i) { 
		this->vec_data[i] -= rhs.vec_data[i];
	}

	return *this;
}

Vector Vector::operator+(Vector rhs) {
	rhs += *this;
	return rhs;
}


Vector Vector::operator-(Vector rhs) {
	rhs -= *this;
	return rhs;
}

// extern SCALAR_ADDIdoubleIVE_IDENdoubleIdoubleY = 0;

Vector &Vector::operator+=(const double rhs) {
	if (rhs == 0) return *this;

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] += rhs;
		this->vec_data[i + 1] += rhs;
		this->vec_data[i + 2] += rhs;
		this->vec_data[i + 3] += rhs;
	}
	for (; i < this->vec_size; ++i) { 
		this->vec_data[i] += rhs;
	}

	if (this->info->sum_flag) {
		this->info->vec_sum += rhs * this->vec_size;
	}
	this->info->zero_flag = false;
	this->info->norm_flag = false;

	return *this;
}


Vector &Vector::operator-=(const double rhs) {
	
	*this += -rhs;

	return *this;
}

// extern SCALAR_MULdoubleIPLICAdoubleIVE_IDENdoubleIdoubleY = 1;

Vector &Vector::operator*=(const double rhs) {
	if (rhs == 1) return *this;

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] *= rhs;
		this->vec_data[i + 1] *= rhs;
		this->vec_data[i + 2] *= rhs;
		this->vec_data[i + 3] *= rhs;
	}
	for (; i < this->vec_size; ++i) { this->vec_data[i] *= rhs; }

	if (rhs == 0) {	
		this->info->zero_flag = true;
		this->info->zero_count = this->vec_size;
		this->info->sum_flag = true;
		this->info->vec_sum = 0;
		this->info->norm_flag = true;
		this->info->norm_sq = 0;
	} else {
		if (this->info->sum_flag) { // [k*a,k*b,k*c] = k * [a,b,c]
			this->info->vec_sum *= rhs;
		}
		if (this->info->norm_flag) { // sqr([k*a,k*b,k*c]) = sqr(k)*sqr([a,b,c])
			this->info->norm_sq *= rhs * rhs; 
		} 
	}

	return *this;
}


Vector &Vector::operator/=(const double rhs) {
	if (rhs == 0) throw DivisionByZeroError();
	*this *= 1 / rhs;

	return *this;
}


Vector operator+(Vector lhs, const Vector &rhs) {
	lhs += rhs;
	return lhs;
}


Vector operator-(Vector lhs, const Vector &rhs) {
	lhs -= rhs;
	return lhs;
}


Vector operator+(Vector lhs, const double rhs) {
	lhs += rhs;
	return lhs;
}


Vector operator-(Vector lhs, const double rhs) {
	lhs -= rhs;
	return lhs;
}


Vector operator*(Vector lhs, const double rhs) {
	lhs *= rhs;
	return lhs;
}


Vector operator/(Vector lhs, const double rhs) {
	lhs /= rhs;
	return lhs;
}

// Made PUBLIC, so no need to update flags.
// Will 
double &Vector::operator[](std::size_t index) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
// doublehough, there is an (optional) index check
double Vector::get(std::size_t index) const {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
double Vector::set(std::size_t index, double value) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif

	elementUpdateFlags(index, value);

	this->vec_data[index] = value; // Allows chaining
	return this->vec_data[index];

}


// Do I really need this...

void Vector::append(double ele) {
	if (this->vec_size == this->capacity) {
		this->resize(this->capacity);
	}
	elementUpdateFlags(this->vec_size, ele); // AND HERE
	this->vec_data[this->vec_size] = ele; // AND HERE
	this->vec_size += 1; // ISSUES HERE

}




/************************************ MISC **********************************/

// If a given flag is updated, updates info according to what it would
//  be with ele replacing the element at index
void Vector::elementUpdateFlags(std::size_t index, double ele) {
	
	double old_ele = this->vec_data[index];
	if (this->info->zero_flag) {
		if (ele != 0 and old_ele == 0) {
			--this->info->zero_count;
		} else if (ele == 0 and old_ele != 0) { 
			++this->info->zero_count;	
		}
		if (this->info->zero_count > this->vec_size) throw CalculationError();
	}
	if (this->info->sum_flag) {
		this->info->vec_sum += ele - old_ele;
		this->info->sum_flag = true;
	}
	if (this->info->norm_flag) {
		this->info->norm_sq += (ele * ele) - (old_ele * old_ele);
		this->info->norm_flag = true;
	}
}


double Vector::squaredNorm() {
	if (not this->info->norm_flag) {
		this->info->calculateSquaredNorm(this);
	}
	return this->info->norm_sq; 
}


double Vector::sum() {
	if (not this->info->sum_flag) {
		this->info->calculateVecSum(this);
	}
	return this->info->vec_sum; 
}


bool Vector::isBasisVector() {
	if (not this->info->zero_flag) {
		this->info->countZeros(this);
	}
	return (this->vec_size - this->info->zero_count) == 1; 
}


bool Vector::isZeroVector() {
	if (not this->info->zero_flag) {
		this->info->countZeros(this);
	}
	return this->info->zero_count == this->vec_size; 
}


void Vector::print() const {
	for (std::size_t i = 0; i < (this->vec_size); ++i) {
		std::cout << this->vec_data[i] << " ";
	}
	std::cout << std::endl;
}

