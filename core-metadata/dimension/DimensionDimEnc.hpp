#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>

class DimensionDimEnc {
   public:
    // it's convention that all 0xff means NULL
    static const unsigned char NULL = (unsigned char) 0xff;
    static bool isNull(std::vector<unsigned char> bytes, int offset, int length) {
        // all 0xff is NULL
        if (length == 0) {
            return false;
        }
        for (int i = 0; i < length; i++) {
            if (bytes[i + offset] != NULL)
                return false;
        }
        return true;
    }
    static std::vector<std::string> parseEncodingConf(const std::string& encoding) {
        std::vector<std::string> results;
        // boost::algorithm::split(results, encoding, is_any_of(":"));
        int cut = encoding.rfind(':');
        if (cut == std::string::npos) {
            // no args
            results.push_back(encoding)
        } else {
            // this is an arg
            results.push_back(encoding.substr(0, cut));
            results.push_back(encoding.substr(cut + 1));
        }
        return results;
    }

    /** return the fixed length of encoded bytes */
    virtual int getLengthOfEncoding() = 0;

    /** encode given value to bytes, note the NULL convention */
    virtual void encode(const std::string& value, std::vector<unsigned char> output, int outputOffset) = 0;

    /** decode given bytes to value string, note the NULL convention */
    virtual std::string decode(std::vector<unsigned char> bytes, int offset, int len) = 0;

    /** return a DataTypeSerializer that does the same encoding/decoding on ByteBuffer */
    // virtual DataTypeSerializer<Object> asDataTypeSerializer();
};