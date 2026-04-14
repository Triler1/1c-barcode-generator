#include"encoder_factory.h"

IEncoder* EncoderFactory::Create(const std::string& type) {
	if (type == "Aztec") {
		return new AztecEncoder();
	}
	throw std::invalid_argument("Unknown encoder type: " + type);
}