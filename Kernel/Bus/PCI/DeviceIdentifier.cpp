/*
 * Copyright (c) 2020, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/AnyOf.h>
#include <AK/Error.h>
#include <AK/NonnullRefPtr.h>
#include <AK/RefPtr.h>
#include <Kernel/Bus/PCI/Definitions.h>

namespace Kernel::PCI {

ErrorOr<NonnullRefPtr<DeviceIdentifier>> DeviceIdentifier::from_enumerable_identifier(EnumerableDeviceIdentifier const& other_identifier)
{
    return adopt_nonnull_ref_or_enomem(new (nothrow) DeviceIdentifier(other_identifier));
}

void DeviceIdentifier::initialize()
{
    for (auto cap : capabilities()) {
        if (cap.id() == PCI::Capabilities::ID::MSIX) {
            auto msix_bir_bar = (cap.read8(4) & msix_table_bir_mask);
            auto msix_bir_offset = (cap.read32(4) & msix_table_offset_mask);
            auto msix_count = (cap.read16(2) & msix_control_table_mask) + 1;
            m_msix_info = MSIxInfo(msix_count, msix_bir_bar, msix_bir_offset);
        }
    }
}

}
