#include"encoder_factory.h"
#include <stdexcept>
IEncoder* EncoderFactory::Create(const std::string& type) {
	if (type == "Aztec") {
		return new AztecEncoder();
	}
	if (type == "QR") {
		return new QREncoder();
	if (type == "DataMatrix") {
		return new DataMatrixEncoder();
	throw std::invalid_argument("Unknown encoder type: " + type);
}