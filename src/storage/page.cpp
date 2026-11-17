#include "storage/page.h"

namespace zeliadb {

Page make_empty_page() {
    Page page{};
    page.fill(0);
    return page;
}

}  // namespace zeliadb
