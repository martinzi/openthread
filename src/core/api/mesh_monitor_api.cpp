/*
 *  Copyright (c) 2026, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "openthread-core-config.h"

#include "instance/instance.hpp"

using namespace ot;

#if OPENTHREAD_CONFIG_MESH_MONITOR_CLIENT_ENABLE

otError otMeshMonGetNextContext(const otMessage *aMessage, otMeshMonIterator *aIterator, otMeshMonContext *aContext)
{
    AssertPointerIsNotNull(aMessage);
    AssertPointerIsNotNull(aIterator);
    AssertPointerIsNotNull(aContext);

    return MeshMonitor::Client::GetNextContext(AsCoapMessage(aMessage), *aIterator, *aContext);
}

otError otMeshMonGetNextTlv(const otMessage *aMessage, otMeshMonContext *aContext, otMeshMonTlv *aTlv)
{
    AssertPointerIsNotNull(aMessage);
    AssertPointerIsNotNull(aContext);
    AssertPointerIsNotNull(aTlv);

    return MeshMonitor::Client::GetNextTlv(AsCoapMessage(aMessage), *aContext, *aTlv);
}

otError otMeshMonGetNextIp6Address(const otMessage          *aMessage,
                                   otMeshMonIp6AddrIterator *aIterator,
                                   otIp6Address             *aAddress)
{
    AssertPointerIsNotNull(aMessage);
    AssertPointerIsNotNull(aIterator);
    AssertPointerIsNotNull(aAddress);

    return MeshMonitor::Client::GetNextIp6Address(AsCoapMessage(aMessage), *aIterator, AsCoreType(aAddress));
}

otError otMeshMonGetNextAloc(const otMessage *aMessage, otMeshMonAlocIterator *aIterator, uint16_t *aAloc)
{
    AssertPointerIsNotNull(aMessage);
    AssertPointerIsNotNull(aIterator);
    AssertPointerIsNotNull(aAloc);

    return MeshMonitor::Client::GetNextAloc(AsCoapMessage(aMessage), *aIterator, *aAloc);
}

otError otMeshMonStartClient(otInstance                   *aInstance,
                             const otMeshMonTlvSet        *aHost,
                             const otMeshMonTlvSet        *aChild,
                             const otMeshMonTlvSet        *aNeighbor,
                             const otIp6Address           *aDestination,
                             otMeshMonServerUpdateCallback aCallback,
                             void                         *aContext)
{
    Error error = kErrorNone;

    VerifyOrExit(aCallback != nullptr, error = kErrorInvalidArgs);

    if (aDestination != nullptr)
    {
        const Ip6::Address &destination = AsCoreType(aDestination);

        VerifyOrExit(!destination.IsUnspecified() && !destination.IsMulticast(), error = kErrorInvalidArgs);
    }

    AsCoreType(aInstance).Get<MeshMonitor::Client>().Start(AsCoreTypePtr(aHost), AsCoreTypePtr(aChild),
                                                           AsCoreTypePtr(aNeighbor), AsCoreTypePtr(aDestination),
                                                           aCallback, aContext);

exit:
    return error;
}

void otMeshMonStopClient(otInstance *aInstance) { AsCoreType(aInstance).Get<MeshMonitor::Client>().Stop(); }

bool otMeshMonTlvIsSet(const otMeshMonTlvSet *aTlvSet, uint8_t aTlv)
{
    bool set = false;

    VerifyOrExit(aTlvSet != nullptr);
    VerifyOrExit(MeshMonitor::Tlv::IsKnownTlv(aTlv));

    set = AsCoreType(aTlvSet).IsSet(static_cast<MeshMonitor::Tlv::Type>(aTlv));

exit:
    return set;
}

otError otMeshMonSetTlv(otMeshMonTlvSet *aTlvSet, uint8_t aTlv)
{
    Error error = kErrorNone;

    VerifyOrExit(aTlvSet != nullptr, error = kErrorInvalidArgs);
    VerifyOrExit(MeshMonitor::Tlv::IsKnownTlv(aTlv), error = kErrorInvalidArgs);

    AsCoreType(aTlvSet).Set(static_cast<MeshMonitor::Tlv::Type>(aTlv));

exit:
    return error;
}

void otMeshMonClearTlv(otMeshMonTlvSet *aTlvSet, uint8_t aTlv)
{
    VerifyOrExit(aTlvSet != nullptr);
    VerifyOrExit(MeshMonitor::Tlv::IsKnownTlv(aTlv));

    AsCoreType(aTlvSet).Clear(static_cast<MeshMonitor::Tlv::Type>(aTlv));

exit:
    return;
}

void otMeshMonClearTlvSet(otMeshMonTlvSet *aTlvSet)
{
    VerifyOrExit(aTlvSet != nullptr);

    AsCoreType(aTlvSet).Clear();

exit:
    return;
}

bool otMeshMonTlvSetIsEmpty(const otMeshMonTlvSet *aTlvSet)
{
    bool isEmpty = true;

    VerifyOrExit(aTlvSet != nullptr);

    isEmpty = AsCoreType(aTlvSet).IsEmpty();

exit:
    return isEmpty;
}

void otMeshMonTlvSetUnion(otMeshMonTlvSet *aDst, const otMeshMonTlvSet *aSrc)
{
    VerifyOrExit(aDst != nullptr);
    VerifyOrExit(aSrc != nullptr);

    AsCoreType(aDst).SetAll(AsCoreType(aSrc));

exit:
    return;
}

bool otMeshMonTlvSetsAreEqual(const otMeshMonTlvSet *aFirst, const otMeshMonTlvSet *aSecond)
{
    bool areEqual = (aFirst == aSecond);

    VerifyOrExit(!areEqual);
    VerifyOrExit((aFirst != nullptr) && (aSecond != nullptr));

    areEqual = (AsCoreType(aFirst) == AsCoreType(aSecond));

exit:
    return areEqual;
}

#endif // OPENTHREAD_CONFIG_MESH_MONITOR_CLIENT_ENABLE
