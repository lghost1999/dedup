#include "fastcdc.h"
#include "chunk.h"

#include <math.h>

static uint32_t normal_size(uint32_t minsize, uint32_t avgsize, uint32_t len) {
    uint32_t offset = minsize + FASTCDC_DIVCEIL(minsize, 2);
    if (offset > avgsize) {
        offset = avgsize;
    }
        
    uint32_t size = avgsize - offset;
    if (size > len) {
        return len;
    }
        
    return size;
}

static uint32_t cut(uint8_t *data, uint32_t len, uint32_t minsize, uint32_t maxsize,
                    uint32_t nmlsize, uint32_t maskS, uint32_t maskL) {
    uint32_t n, fp = 0, i = (len < minsize) ? len : minsize;

    n = (nmlsize < len) ? nmlsize : len;
    for (; i < n; i++) {                 
        fp = (fp >> 1) + GEAR[data[i]];
        if ((fp & maskS) == 0) {
            return i + 1;
        }
    }

    n = (maxsize < len) ? maxsize : len;
    for (; i < n; i++) {                 
        fp = (fp >> 1) + GEAR[data[i]];
        if ((fp & maskL) == 0) {
            return i + 1;
        }
    }

    return i;
}


FastCDC::FastCDC(uint32_t minsize, uint32_t avgsize, uint64_t maxsize) {
    uint32_t bits = (uint32_t)round(log2(avgsize));
    minsize_ = FASTCDC_CLAMP(minsize, MINIMUM_MIN, MINIMUM_MAX);
    avgsize_ = FASTCDC_CLAMP(avgsize, AVERAGE_MIN, AVERAGE_MAX);
    maxsize_ = FASTCDC_CLAMP(maxsize, MAXIMUM_MIN, MAXIMUM_MAX);
    nmlsize_ = normal_size(minsize, avgsize, maxsize);
    maskS_ = FASTCDC_MASK(bits + 1);
    maskL_ = FASTCDC_MASK(bits - 1);
    pos_ = 0;
}

size_t FastCDC::chunking(char* path, uint8_t *data, size_t len, int end, std::vector<Chunk>& chunks) {
    size_t offset = 0;
    while (((len - offset) >= maxsize_) || (end && (offset < len))) {
        uint32_t chunklen = cut(data + offset, len - offset, minsize_, 
                                maxsize_, nmlsize_, maskS_, maskL_);
        Fringerprint fringerprint = Fringerprint::get(data, chunklen, 0);
        chunks.emplace_back(Chunk(path, pos_ + offset, chunklen, 0, fringerprint));
        offset += chunklen;
    }

    pos_ += offset;
    return offset;
}

bool FastCDC::parse(char* path, std::vector<Chunk>& chunks) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return false;
    }

    size_t offset = 0;
    int end = 0;
    size_t bufsize = maxsize_ * 4;
    bufsize = FASTCDC_CLAMP(bufsize, maxsize_, UINT32_MAX);
    uint8_t buf[bufsize];
    while (!end) {
        size_t ar = fread(buf, 1, bufsize, file);
        end = feof(file);
        offset += chunking(path, buf, ar, end, chunks);
        fseek(file, offset, SEEK_SET);
    }

    fclose(file);
    return true;
}

uint32_t FastCDC::getMinsize() const {
    return minsize_;
}

void FastCDC::setMinsize(uint32_t minsize) {
    minsize_ = minsize;
}

uint32_t FastCDC::getAvgsize() const {
    return avgsize_;
}

void FastCDC::setAvgsize(uint32_t avgsize) {
    avgsize_ = avgsize;
}

uint32_t FastCDC::getMaxsize() const {
    return maxsize_;
}

void FastCDC::setMaxsize(uint32_t maxsize) {
    maxsize_ = maxsize;
}

size_t FastCDC::getPos() const {
    return pos_;
}

void FastCDC::setPos(size_t pos) {
    pos_ = pos;
}

void FastCDC::clear() {
    pos_ = 0;
}
