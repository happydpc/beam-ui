// Copyright 2019 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "swap_offer_item.h"
#include "utility/helpers.h"
#include "ui_helpers.h"
#include "wallet/common.h"

SwapOfferItem::SwapOfferItem(const SwapOffer& offer, bool isOwn)
    : m_offer{offer},
      m_isOwnOffer{isOwn}
{
    m_offer.GetParameter(TxParameterID::AtomicSwapIsBeamSide, m_isBeamSide);
}

QDateTime SwapOfferItem::timeCreated() const
{
    beam::Timestamp time;
    if (m_offer.GetParameter(TxParameterID::CreateTime, time))
    {
        QDateTime datetime;
        datetime.setTime_t(time);
        return datetime;
    }
    else
    {
        return QDateTime();
    }
}

QDateTime SwapOfferItem::timeExpiration() const
{
    beam::Timestamp time;
    if (m_offer.GetParameter(TxParameterID::CreateTime, time))
    {
        QDateTime datetime;
        time += 60*60*24;       // defaut lifetime 24h
        datetime.setTime_t(time);
        // TODO : minus lifetime
        // if (m_offer.GetParameter(beam::wallet::TxParameterID::PeerResponseHeight, x))
        return datetime;
    }
    else
    {
        return QDateTime();
    }
}

beam::Amount SwapOfferItem::rawAmount() const
{
    beam::wallet::TxParameterID paramID = m_isBeamSide ? TxParameterID::AtomicSwapAmount : TxParameterID::Amount;
    beam::Amount amount;
    if (m_offer.GetParameter(paramID, amount))
    {
        return amount;
    }
    return 0;
}

beam::Amount SwapOfferItem::rawAmountSwap() const
{
    beam::wallet::TxParameterID paramID = m_isBeamSide ? TxParameterID::Amount : TxParameterID::AtomicSwapAmount;
    beam::Amount amount;
    if (m_offer.GetParameter(paramID, amount))
    {
        return amount;
    }
    return 0;
}

double SwapOfferItem::rate() const
{
    double amountSwap = double(int64_t(rawAmountSwap()));
    double amount = double(int64_t(rawAmount()));
    double rate = amountSwap / amount;

    double p = pow( 10., 7 );
    return floor( rate * p + .5 ) / p;
}

QString SwapOfferItem::amount() const
{
    auto coinType = getCoinType();
    return beamui::AmountToString(rawAmount(), coinType);
}

QString SwapOfferItem::amountSwap() const
{
    auto coinType = getSwapCoinType();
    return beamui::AmountToString(rawAmountSwap(), coinType);
}

bool SwapOfferItem::isOwnOffer() const
{
    return m_isOwnOffer;
}

beam::wallet::TxParameters SwapOfferItem::getTxParameters() const
{
    return m_offer;
}

auto SwapOfferItem::getSwapCoinType() const -> beamui::Currencies
{
    if (m_isBeamSide)
    {
        return beamui::Currencies::Beam;
    }
    else
    {
        beam::wallet::AtomicSwapCoin coin;
        if (m_offer.GetParameter(TxParameterID::AtomicSwapCoin, coin))
        {
            switch (coin)
            {
                case AtomicSwapCoin::Bitcoin:   return beamui::Currencies::Bitcoin;
                case AtomicSwapCoin::Litecoin:  return beamui::Currencies::Bitcoin;
                case AtomicSwapCoin::Qtum:      return beamui::Currencies::Bitcoin;
            }
        }
        return beamui::Currencies::Unknown;
    } 
}

auto SwapOfferItem::getCoinType() const -> beamui::Currencies
{
    if (!m_isBeamSide)
    {
        return beamui::Currencies::Beam;
    }
    else
    {
        beam::wallet::AtomicSwapCoin coin;
        if (m_offer.GetParameter(TxParameterID::AtomicSwapCoin, coin))
        {
            switch (coin)
            {
                case AtomicSwapCoin::Bitcoin:   return beamui::Currencies::Bitcoin;
                case AtomicSwapCoin::Litecoin:  return beamui::Currencies::Bitcoin;
                case AtomicSwapCoin::Qtum:      return beamui::Currencies::Bitcoin;
            }
        }
        return beamui::Currencies::Unknown;
    }    
}
