#include "kylin/dimensionDimEnc.hpp"
#include "kylin/utils.hpp"

class IntegerDimEnc : public DimensionDimEnc {
   public:
    IntegerDimEnc(int len);
    ~IntegerDimEnc();
    static const std::string ENCODING_NAME = "integer";
    /** return the fixed length of encoded bytes */
    virtual int getLengthOfEncoding();
    /** encode given value to bytes, note the NULL convention */
    virtual void encode(const std::string& valueStr, std::vector<unsigned char> output, int outputOffset);
    /** decode given bytes to value string, note the NULL convention */
    virtual std::string decode(std::vector<unsigned char> bytes, int offset, int len);

   private:
    static const long[] CAP = {0,
                               0x7fL,
                               0x7fffL,
                               0x7fffffL,
                               0x7fffffffL,
                               0x7fffffffffL,
                               0x7fffffffffffL,
                               0x7fffffffffffffL,
                               0x7fffffffffffffffL};
    static const long[] MASK = {0,
                                0xffL,
                                0xffffL,
                                0xffffffL,
                                0xffffffffL,
                                0xffffffffffL,
                                0xffffffffffffL,
                                0xffffffffffffffL,
                                0xffffffffffffffffL};
    static const long[] TAIL = {0,
                                0x80L,
                                0x8000L,
                                0x800000L,
                                0x80000000L,
                                0x8000000000L,
                                0x800000000000L,
                                0x80000000000000L,
                                0x8000000000000000L};
    int fixedLen;
};