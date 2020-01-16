// Copyright 2019 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "eth_params.h"

#include <util.h>

#include <wally_bip32.h>

static const app_eth_coin_params_t _params_eth = {
    .bip44_coin = 60 + BIP32_INITIAL_HARDENED_CHILD,
    .chain_id = 1,
    .unit = "ETH",
};

static const app_eth_coin_params_t _params_ropsten_eth = {
    .bip44_coin = 1 + BIP32_INITIAL_HARDENED_CHILD,
    .chain_id = 3,
    .unit = "TETH",
};

static const app_eth_coin_params_t _params_rinkeby_eth = {
    .bip44_coin = 1 + BIP32_INITIAL_HARDENED_CHILD,
    .chain_id = 4,
    .unit = "TETH",
};

const app_eth_coin_params_t* app_eth_params_get(ETHCoin coin)
{
    switch (coin) {
    case ETHCoin_ETH:
        return &_params_eth;
    case ETHCoin_RopstenETH:
        return &_params_ropsten_eth;
    case ETHCoin_RinkebyETH:
        return &_params_rinkeby_eth;
    default:
        return NULL;
    }
}

static const app_eth_erc20_params_t _erc20_params[] = {
    /* Ropsten */
    {
        .coin = ETHCoin_RopstenETH,
        .name = "TEST",
        .unit = "TEST",
        .contract_address =
            "\x2f\x45\xb6\xfb\x2f\x28\xa7\x3f\x11\x04\x00\x38\x6d\xa3\x10\x44\xb2\xe9\x53\xd4",
        .decimals = 18,
    },
    /* Ethereum */
    {
        .coin = ETHCoin_ETH,
        .name = "0xBitcoin Token",
        .unit = "0xBTC",
        .contract_address =
            "\xb6\xed\x76\x44\xc6\x94\x16\xd6\x7b\x52\x2e\x20\xbc\x29\x4a\x9a\x9b\x40\x5b\x31",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "1World",
        .unit = "1WO",
        .contract_address =
            "\xfd\xbc\x1a\xdc\x26\xf0\xf8\xf8\x60\x6a\x5d\x63\xb7\xd3\xa3\xcd\x21\xc2\x2b\x23",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "300 Token",
        .unit = "300",
        .contract_address =
            "\xae\xc9\x8a\x70\x88\x10\x41\x48\x78\xc3\xbc\xdf\x46\xaa\xd3\x1d\xed\x4a\x45\x57",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Alpha",
        .unit = "A",
        .contract_address =
            "\xff\xc6\x3b\x91\x46\x96\x7a\x1b\xa3\x30\x66\xfb\x05\x7e\xe3\x72\x22\x21\xac\xf0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Change Your Life",
        .unit = "AAA",
        .contract_address =
            "\xd9\x38\x13\x7e\x6d\x96\xc7\x2e\x4a\x60\x85\x41\x2a\xda\x2d\xad\x78\xff\x89\xc4",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Airbloc",
        .unit = "ABL",
        .contract_address =
            "\xf8\xb3\x58\xb3\x39\x7a\x8e\xa5\x46\x4f\x8c\xc7\x53\x64\x5d\x42\xe1\x4b\x79\xea",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ArcBlock",
        .unit = "ABT",
        .contract_address =
            "\xb9\x8d\x4c\x97\x42\x5d\x99\x08\xe6\x6e\x53\xa6\xfd\xf6\x73\xac\xca\x0b\xe9\x86",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ABX Token",
        .unit = "ABX",
        .contract_address =
            "\x9a\x79\x4d\xc1\x93\x9f\x1d\x78\xfa\x48\x61\x3b\x89\xb8\xf9\xd0\xa2\x0d\xa0\x0e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Abyss",
        .unit = "ABYSS",
        .contract_address =
            "\x0e\x8d\x6b\x47\x1e\x33\x2f\x14\x0e\x7d\x9d\xbb\x99\xe5\xe3\x82\x2f\x72\x8d\xa6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Accelerator",
        .unit = "ACC",
        .contract_address =
            "\x13\xf1\xb7\xfd\xfb\xe1\xfc\x66\x67\x6d\x56\x48\x3e\x21\xb1\xec\xb4\x0b\x58\xe2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ACE",
        .unit = "ACE",
        .contract_address =
            "\x06\x14\x71\x10\x02\x2b\x76\x8b\xa8\xf9\x9a\x8f\x38\x5d\xf1\x1a\x15\x1a\x9c\xc8",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Archetypal Network",
        .unit = "ACTP",
        .contract_address =
            "\x7b\x2d\xf1\x25\x56\x78\x15\xac\x9b\x57\xda\x04\xb6\x20\xf5\x0b\xc9\x3b\x32\x0c",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AdBank",
        .unit = "ADB",
        .contract_address =
            "\x2b\xaa\xc9\x33\x0c\xf9\xac\x47\x9d\x81\x91\x95\x79\x4d\x79\xad\x0c\x76\x16\xe3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aditus",
        .unit = "ADI",
        .contract_address =
            "\x88\x10\xc6\x34\x70\xd3\x86\x39\x95\x4c\x6b\x41\xaa\xc5\x45\x84\x8c\x46\x48\x4a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Adelphoi",
        .unit = "ADL",
        .contract_address =
            "\x66\x0e\x71\x48\x37\x85\xf6\x61\x33\x54\x8B\x10\xf6\x92\x6d\xC3\x32\xb0\x6e\x61",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Adshares",
        .unit = "ADST",
        .contract_address =
            "\x42\x28\x66\xa8\xf0\xb0\x32\xc5\xcf\x1d\xfb\xde\xf3\x1a\x20\xf4\x50\x95\x62\xb0",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AdToken",
        .unit = "ADT",
        .contract_address =
            "\xd0\xd6\xd6\xc5\xfe\x4a\x67\x7d\x34\x3c\xc4\x33\x53\x6b\xb7\x17\xba\xe1\x67\xdd",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AdEx",
        .unit = "ADX",
        .contract_address =
            "\x44\x70\xBB\x87\xd7\x7b\x96\x3A\x01\x3D\xB9\x39\xBE\x33\x2f\x92\x7f\x2b\x99\x2e",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aeternity",
        .unit = "AE",
        .contract_address =
            "\x5c\xa9\xa7\x1b\x1d\x01\x84\x9c\x0a\x95\x49\x0c\xc0\x05\x59\x71\x7f\xcf\x0d\x1d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AEN.",
        .unit = "AEN",
        .contract_address =
            "\x0b\xef\x61\x9c\xf3\x8c\xf0\xc2\x29\x67\x28\x9b\x84\x19\x72\x0f\xbd\x1d\xb9\xf7",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aergo",
        .unit = "AERGO",
        .contract_address =
            "\xae\x31\xb8\x5b\xfe\x62\x74\x7d\x08\x36\xb8\x26\x08\xb4\x83\x03\x61\xa3\xd3\x7a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SingularityNET",
        .unit = "AGI",
        .contract_address =
            "\x8e\xb2\x43\x19\x39\x37\x16\x66\x8d\x76\x8d\xce\xc2\x93\x56\xae\x9c\xff\xe2\x85",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Agrolot Token",
        .unit = "AGLT",
        .contract_address =
            "\x72\xc9\xfb\x7e\xd1\x9d\x3c\xe5\x1c\xea\x5c\x56\xb3\xe0\x23\xcd\x91\x8b\xaa\xdf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PolyAi",
        .unit = "AI",
        .contract_address =
            "\x51\x21\xe3\x48\xe8\x97\xda\xef\x1e\xef\x23\x95\x9a\xb2\x90\xe5\x55\x7c\xf2\x74",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AidCoin",
        .unit = "AID",
        .contract_address =
            "\x37\xe8\x78\x9b\xb9\x99\x6c\xac\x91\x56\xcd\x5f\x5f\xd3\x25\x99\xe6\xb9\x12\x89",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AION",
        .unit = "AION",
        .contract_address =
            "\x4C\xEd\xA7\x90\x6a\x5E\xd2\x17\x97\x85\xCd\x3A\x40\xA6\x9e\xe8\xbc\x99\xC4\x66",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AirToken",
        .unit = "AIR",
        .contract_address =
            "\x27\xdc\xe1\xec\x4d\x3f\x72\xc3\xe4\x57\xcc\x50\x35\x4f\x1f\x97\x5d\xde\xf4\x88",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aigang",
        .unit = "AIX",
        .contract_address =
            "\x10\x63\xce\x52\x42\x65\xd5\xa3\xa6\x24\xf4\x91\x4a\xcd\x57\x3d\xd8\x9c\xe9\x88",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Akropolis",
        .unit = "AKRO",
        .contract_address =
            "\x8a\xb7\x40\x40\x63\xec\x4d\xbc\xfd\x45\x98\x21\x59\x92\xdc\x3f\x8e\xc8\x53\xd7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AiLink Token",
        .unit = "ALI",
        .contract_address =
            "\x42\x89\xc0\x43\xa1\x23\x92\xf1\x02\x73\x07\xfb\x58\x27\x2d\x8e\xbd\x85\x39\x12",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Alis",
        .unit = "ALIS",
        .contract_address =
            "\xea\x61\x0b\x11\x53\x47\x77\x20\x74\x8d\xc1\x3e\xd3\x78\x00\x39\x41\xd8\x4f\xab",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AltEstate token",
        .unit = "ALT",
        .contract_address =
            "\x41\x9b\x8e\xd1\x55\x18\x0a\x8c\x9c\x64\x14\x5e\x76\xda\xd4\x9c\x0a\x4e\xfb\x97",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ALX",
        .unit = "ALX",
        .contract_address =
            "\x49\xb1\x27\xbc\x33\xce\x7e\x15\x86\xec\x28\xce\xc6\xa6\x5b\x11\x25\x96\xc8\x22",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Amber",
        .unit = "AMB",
        .contract_address =
            "\x4d\xc3\x64\x3d\xbc\x64\x2b\x72\xc1\x58\xe7\xf3\xd2\xff\x23\x2d\xf6\x1c\xb6\xce",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AMLT",
        .unit = "AMLT",
        .contract_address =
            "\xca\x0e\x72\x69\x60\x0d\x35\x3f\x70\xb1\x4a\xd1\x18\xa4\x95\x75\x45\x5c\x0f\x2f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Amon",
        .unit = "AMN",
        .contract_address =
            "\x73\x7f\x98\xac\x8c\xa5\x9f\x2c\x68\xad\x65\x8e\x3c\x3d\x8c\x89\x63\xe4\x0a\x4c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ampleforth",
        .unit = "AMPL",
        .contract_address =
            "\xd4\x6b\xa6\xd9\x42\x05\x0d\x48\x9d\xbd\x93\x8a\x2c\x90\x9a\x5d\x50\x39\xa1\x61",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ankr Network",
        .unit = "ANKR",
        .contract_address =
            "\x82\x90\x33\x3c\xef\x9e\x6d\x52\x8d\xd5\x61\x8f\xb9\x7a\x76\xf2\x68\xf3\xed\xd4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aragon",
        .unit = "ANT",
        .contract_address =
            "\x96\x0b\x23\x6A\x07\xcf\x12\x26\x63\xc4\x30\x33\x50\x60\x9A\x66\xA7\xB2\x88\xC0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aurora",
        .unit = "AOA",
        .contract_address =
            "\x9a\xb1\x65\xd7\x95\x01\x9b\x6d\x8b\x3e\x97\x1d\xda\x91\x07\x14\x21\x30\x5e\x5a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AOG",
        .unit = "AOG",
        .contract_address =
            "\x85\x78\x53\x02\x05\xce\xcb\xe5\xdb\x83\xf7\xf2\x9e\xcf\xee\xc8\x60\xc2\x97\xc2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Alpha Coin",
        .unit = "APC",
        .contract_address =
            "\x15\xbd\xa0\x8c\x3a\xfb\xf5\x95\x5d\x6e\x9b\x23\x5f\xd5\x5a\x1f\xd0\xdb\xc8\x29",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "APIS",
        .unit = "APIS",
        .contract_address =
            "\x4c\x0f\xbe\x1b\xb4\x66\x12\x91\x5e\x79\x67\xd2\xc3\x21\x3c\xd4\xd8\x72\x57\xad",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AppCoins",
        .unit = "APPC",
        .contract_address =
            "\x1a\x7a\x8b\xd9\x10\x6f\x2b\x8d\x97\x7e\x08\x58\x2d\xc7\xd2\x4c\x72\x3a\xb0\xdb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ARBITRAGE",
        .unit = "ARB",
        .contract_address =
            "\xaf\xbe\xc4\xd6\x5b\xc7\xb1\x16\xd8\x51\x07\xfd\x05\xd9\x12\x49\x10\x29\xbf\x46",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Arcade",
        .unit = "ARC",
        .contract_address =
            "\xAc\x70\x9F\xcB\x44\xa4\x3c\x35\xF0\xDA\x4e\x31\x63\xb1\x17\xA1\x7F\x37\x70\xf5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ArbitrageCT",
        .unit = "ARCT",
        .contract_address =
            "\x12\x45\xef\x80\xf4\xd9\xe0\x2e\xd9\x42\x53\x75\xe8\xf6\x49\xb9\x22\x1b\x31\xd8",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aeron",
        .unit = "ARN",
        .contract_address =
            "\xBA\x5F\x11\xb1\x6B\x15\x57\x92\xCf\x3B\x2E\x68\x80\xE8\x70\x68\x59\xA8\xAE\xB6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Maecenas ART Token",
        .unit = "ART",
        .contract_address =
            "\xfe\xc0\xcf\x7f\xe0\x78\xa5\x00\xab\xf1\x5f\x12\x84\x95\x8f\x22\x04\x9c\x2c\x7e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ArtisTurba",
        .unit = "ARTIS",
        .contract_address =
            "\x08\x2e\x13\x49\x4f\x12\xeb\xb7\x20\x6f\xbf\x67\xe2\x2a\x6e\x19\x75\xa1\xa6\x69",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Block Array",
        .unit = "ARY",
        .contract_address =
            "\xa5\xf8\xfc\x09\x21\x88\x0c\xb7\x34\x23\x68\xbd\x12\x8e\xb8\x05\x04\x42\xb1\xa1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AirSwap",
        .unit = "AST",
        .contract_address =
            "\x27\x05\x4b\x13\xb1\xb7\x98\xb3\x45\xb5\x91\xa4\xd2\x2e\x65\x62\xd4\x7e\xa7\x5a",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AstroTokens",
        .unit = "ASTRO",
        .contract_address =
            "\x7b\x22\x93\x8c\xa8\x41\xaa\x39\x2c\x93\xdb\xb7\xf4\xc4\x21\x78\xe3\xd6\x5e\x88",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ATLANT",
        .unit = "ATL",
        .contract_address =
            "\x78\xb7\xfa\xda\x55\xa6\x4d\xd8\x95\xd8\xc8\xc3\x57\x79\xdd\x8b\x67\xfa\x8a\x05",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Atonomi",
        .unit = "ATMI",
        .contract_address =
            "\x97\xae\xb5\x06\x6e\x1a\x59\x0e\x86\x8b\x51\x14\x57\xbe\xb6\xfe\x99\xd3\x29\xf5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ATN",
        .unit = "ATN",
        .contract_address =
            "\x46\x17\x33\xc1\x7b\x07\x55\xca\x56\x49\xb6\xdb\x08\xb3\xe2\x13\xfc\xf2\x25\x46",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Authorship",
        .unit = "ATS",
        .contract_address =
            "\x2d\xAE\xE1\xAA\x61\xD6\x0A\x25\x2D\xC8\x05\x64\x49\x9A\x69\x80\x28\x53\x58\x3A",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aston X",
        .unit = "ATX",
        .contract_address =
            "\x1a\x0f\x2a\xb4\x6e\xc6\x30\xf9\xfd\x63\x80\x29\x02\x7b\x55\x2a\xfa\x64\xb9\x4c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Auctus",
        .unit = "AUC",
        .contract_address =
            "\xc1\x2d\x09\x9b\xe3\x15\x67\xad\xd4\xe4\xe4\xd0\xd4\x56\x91\xc3\xf5\x8f\x56\x63",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Aurora DAO",
        .unit = "AURA",
        .contract_address =
            "\xcd\xcf\xc0\xf6\x6c\x52\x2f\xd0\x86\xa1\xb7\x25\xea\x3c\x0e\xeb\x9f\x9e\x88\x14",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CUBE",
        .unit = "AUTO",
        .contract_address =
            "\x62\x2d\xFf\xCc\x4e\x83\xC6\x4b\xa9\x59\x53\x0A\x5a\x55\x80\x68\x7a\x57\x58\x1b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AVT",
        .unit = "AVT",
        .contract_address =
            "\x0d\x88\xed\x6e\x74\xbb\xfd\x96\xb8\x31\x23\x16\x38\xb6\x6c\x05\x57\x1e\x82\x4f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Atomic Wallet Token",
        .unit = "AWC",
        .contract_address =
            "\xad\x22\xf6\x34\x04\xf7\x30\x5e\x47\x13\xcc\xbd\x4f\x29\x6f\x34\x77\x05\x13\xf4",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AXNET",
        .unit = "AXN",
        .contract_address =
            "\x30\x42\x81\xf3\xd1\x02\x3a\x20\x39\xea\x93\x0c\x65\xf8\xf7\x21\xd7\xc7\x46\xc8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "aXpire",
        .unit = "AXPR",
        .contract_address =
            "\xC3\x9E\x62\x6A\x04\xC5\x97\x1D\x77\x0e\x31\x97\x60\xD7\x92\x65\x02\x97\x5e\x47",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "B2BX",
        .unit = "B2BX",
        .contract_address =
            "\x5d\x51\xfc\xce\xd3\x11\x4a\x8b\xb5\xe9\x0c\xdd\x0f\x9d\x68\x2b\xcb\xcc\x53\x93",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BaaSid",
        .unit = "BAAS",
        .contract_address =
            "\x3e\x65\xe1\xee\xfd\xe5\xea\x7c\xcf\xc9\xa9\xa1\x63\x4a\xbe\x90\xf3\x22\x62\xf8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BANCA",
        .unit = "BANCA",
        .contract_address =
            "\x99\x8b\x3b\x82\xbc\x9d\xba\x17\x39\x90\xbe\x7a\xfb\x77\x27\x88\xb5\xac\xb8\xbd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BandToken",
        .unit = "BAND",
        .contract_address =
            "\xba\x11\xd0\x0c\x5f\x74\x25\x5f\x56\xa5\xe3\x66\xf4\xf7\x7f\x5a\x18\x6d\x7f\x55",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitAsean",
        .unit = "BAS",
        .contract_address =
            "\x2a\x05\xd2\x2d\xb0\x79\xbc\x40\xc2\xf7\x7a\x1d\x1f\xf7\x03\xa5\x6e\x63\x1c\xc1",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Basic Attention Token",
        .unit = "BAT",
        .contract_address =
            "\x0d\x87\x75\xf6\x48\x43\x06\x79\xa7\x09\xe9\x8d\x2b\x0c\xb6\x25\x0d\x28\x87\xef",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BAX",
        .unit = "BAX",
        .contract_address =
            "\x9a\x02\x42\xb7\xa3\x3d\xac\xbe\x40\xed\xb9\x27\x83\x4f\x96\xeb\x39\xf8\xfb\xcb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "B2BCoin",
        .unit = "BBC",
        .contract_address =
            "\xe7\xD3\xe4\x41\x3E\x29\xae\x35\xB0\x89\x31\x40\xF4\x50\x09\x65\xc7\x43\x65\xe5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Beluga Banking Infrastructure Token",
        .unit = "BBI",
        .contract_address =
            "\x37\xd4\x05\x10\xa2\xf5\xbc\x98\xaa\x7a\x0f\x7b\xf4\xb3\x45\x3b\xcf\xb9\x0a\xc1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Brickblock",
        .unit = "BBK",
        .contract_address =
            "\x4a\x60\x58\x66\x6c\xf1\x05\x7e\xac\x3c\xd3\xa5\xa6\x14\x62\x05\x47\x55\x9f\xc9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BBN",
        .unit = "BBN",
        .contract_address =
            "\x35\xa6\x96\x42\x85\x70\x83\xba\x2f\x30\xbf\xab\x73\x5d\xac\xc7\xf0\xba\xc9\x69",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bigbom",
        .unit = "BBO",
        .contract_address =
            "\x84\xf7\xc4\x4b\x6f\xed\x10\x80\xf6\x47\xe3\x54\xd5\x52\x59\x5b\xe2\xcc\x60\x2f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Block-Chain.com Token",
        .unit = "BC",
        .contract_address =
            "\x2e\xcb\x13\xa8\xc4\x58\xc3\x79\xc4\xd9\xa7\x25\x9e\x20\x2d\xe0\x3c\x8f\x3d\x19",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Blockchain Certified Data Token",
        .unit = "BCDT",
        .contract_address =
            "\xac\xfa\x20\x9f\xb7\x3b\xf3\xdd\x5b\xbf\xb1\x10\x1b\x9b\xc9\x99\xc4\x90\x62\xa5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BananaCoin Extended",
        .unit = "BCO",
        .contract_address =
            "\x86\x5d\x17\x63\x51\xf2\x87\xfe\x1b\x00\x10\x80\x5b\x11\x0d\x08\x69\x9c\x20\x0a",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BLOCKMASON CREDIT PROTOCOL TOKEN",
        .unit = "BCPT",
        .contract_address =
            "\x1c\x44\x81\x75\x0d\xaa\x5f\xf5\x21\xa2\xa7\x49\x0d\x99\x81\xed\x46\x46\x5d\xbd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitCapitalVendorToken",
        .unit = "BCV",
        .contract_address =
            "\x10\x14\x61\x3e\x2b\x3c\xbc\x4d\x57\x50\x54\xd4\x98\x2e\x58\x0d\x9b\x99\xd7\xb1",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Buggyra Coin Zero",
        .unit = "BCZERO",
        .contract_address =
            "\xd4\x52\x47\xc0\x73\x79\xd9\x49\x04\xe0\xa8\x7b\x44\x81\xf0\xa1\xdd\xfa\x0c\x64",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitDegree",
        .unit = "BDG",
        .contract_address =
            "\x19\x61\xb3\x33\x19\x69\xed\x52\x77\x07\x51\xfc\x71\x8e\xf5\x30\x83\x8b\x6d\xee",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BDT Token",
        .unit = "BDT",
        .contract_address =
            "\x03\x30\x30\xfe\xeb\xd9\x3e\x31\x78\x48\x7c\x35\xa9\xc8\xca\x80\x87\x43\x53\xc9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BEAT",
        .unit = "BEAT",
        .contract_address =
            "\x2f\xb1\x2b\xcc\xf6\xf5\xdd\x33\x8b\x76\xbe\x78\x4a\x93\xad\xe0\x72\x42\x56\x90",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bee",
        .unit = "BEE",
        .contract_address =
            "\x4d\x8f\xc1\x45\x3a\x0f\x35\x9e\x99\xc9\x67\x59\x54\xe6\x56\xd8\x0d\x99\x6f\xbf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BetProtocolToken",
        .unit = "BEPRO",
        .contract_address =
            "\x78\x60\x01\xc9\xc5\xCA\x6E\x50\x2d\xEB\x8a\x8a\x72\x48\x0d\x21\x47\x89\x1f\x32",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Berry",
        .unit = "BERRY",
        .contract_address =
            "\x6a\xeb\x95\xf0\x6c\xda\x84\xca\x34\x5c\x2d\xe0\xf3\xb7\xf9\x69\x23\xa4\x4f\x4c",
        .decimals = 14,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dao.Casino",
        .unit = "BET",
        .contract_address =
            "\x8a\xa3\x3a\x78\x99\xfc\xc8\xea\x5f\xbe\x6a\x60\x8a\x10\x9c\x38\x93\xa1\xb8\xb2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bethereum",
        .unit = "BETHER",
        .contract_address =
            "\x14\xc9\x26\xf2\x29\x00\x44\xb6\x47\xe1\xbf\x20\x72\xe6\x7b\x49\x5e\xff\x19\x05",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Better Betting",
        .unit = "BETR",
        .contract_address =
            "\x76\x31\x86\xeb\x8d\x48\x56\xd5\x36\xed\x44\x78\x30\x29\x71\x21\x4f\xeb\xc6\xa9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitForex Token",
        .unit = "BF",
        .contract_address =
            "\x5b\x71\xbe\xe9\xd9\x61\xb1\xb8\x48\xf8\x48\x5e\xec\x8d\x87\x87\xf8\x02\x17\xf5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BHPCash",
        .unit = "BHPC",
        .contract_address =
            "\xee\x74\x11\x0f\xb5\xa1\x00\x7b\x06\x28\x2e\x0d\xe5\xd7\x3a\x61\xbf\x41\xd9\xcd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BITPARK",
        .unit = "BITPARK",
        .contract_address =
            "\xf3\xd2\x9f\xb9\x8d\x2d\xc5\xe7\x8c\x87\x19\x8d\xee\xf9\x93\x77\x34\x5f\xd6\xf1",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bitcoinus Token",
        .unit = "BITS",
        .contract_address =
            "\xc3\x8f\x1f\xb4\x9a\xcd\xf2\xf1\x21\x3c\xaf\x33\x19\xf6\xeb\x3e\xa2\xcb\x75\x27",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitScreenerToken",
        .unit = "BITX",
        .contract_address =
            "\xff\x2b\x33\x53\xc3\x01\x5e\x9f\x1f\xbf\x95\xb9\xbd\xa2\x3f\x58\xaa\x7c\xe0\x07",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BIX Token",
        .unit = "BIX",
        .contract_address =
            "\xb3\x10\x4b\x4b\x9d\xa8\x20\x25\xe8\xb9\xf8\xfb\x28\xb3\x55\x3c\xe2\xf6\x70\x69",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BANKEX",
        .unit = "BKX",
        .contract_address =
            "\x45\x24\x5b\xc5\x92\x19\xee\xaa\xf6\xcd\x3f\x38\x2e\x07\x8a\x46\x1f\xf9\xde\x7b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bolenum",
        .unit = "BLN",
        .contract_address =
            "\x0c\x65\x8f\xa2\xea\xa2\x92\xe8\xdc\xa0\x85\xda\xb0\xe5\xcb\xd2\xda\x21\xc4\x94",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bloom",
        .unit = "BLT",
        .contract_address =
            "\x10\x7c\x45\x04\xcd\x79\xc5\xd2\x69\x6e\xa0\x03\x0a\x8d\xd4\xe9\x26\x01\xb8\x2e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethereum Blue",
        .unit = "BLUE",
        .contract_address =
            "\x53\x9e\xfe\x69\xbc\xdd\x21\xa8\x3e\xfd\x91\x22\x57\x1a\x64\xcc\x25\xe0\x28\x2b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bluzelle",
        .unit = "BLZ",
        .contract_address =
            "\x57\x32\x04\x6a\x88\x37\x04\x40\x4f\x28\x4c\xe4\x1f\xfa\xdd\x5b\x00\x7f\xd6\x68",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Blackmoon Crypto Token",
        .unit = "BMC",
        .contract_address =
            "\xdf\x6e\xf3\x43\x35\x07\x80\xbf\x8c\x34\x10\xbf\x06\x2e\x0c\x01\x5b\x1d\xd6\x71",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitMartToken",
        .unit = "BMC",
        .contract_address =
            "\x98\x6E\xE2\xB9\x44\xc4\x2D\x01\x7F\x52\xAf\x21\xc4\xc6\x9B\x84\xDB\xeA\x35\xd8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Chimpion",
        .unit = "BNANA",
        .contract_address =
            "\x07\xef\x9e\x82\x72\x1a\xc1\x68\x09\xd2\x4d\xaf\xbe\x17\x92\xce\x01\x65\x4d\xb4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BNB",
        .unit = "BNB",
        .contract_address =
            "\xB8\xc7\x74\x82\xe4\x5F\x1F\x44\xdE\x17\x45\xF5\x2C\x74\x42\x6C\x63\x1b\xDD\x52",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bionic",
        .unit = "BNC",
        .contract_address =
            "\xef\x51\xc9\x37\x7f\xeb\x29\x85\x6e\x61\x62\x5c\xaf\x93\x90\xbd\x0b\x67\xea\x18",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Banker Token",
        .unit = "BNK",
        .contract_address =
            "\xc8\x0c\x5e\x40\x22\x01\x72\xb3\x6a\xde\xe2\xc9\x51\xf2\x6f\x2a\x57\x78\x10\xc5",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BrokerNekoNetwork",
        .unit = "BNN",
        .contract_address =
            "\xda\x80\xb2\x00\x38\xbd\xf9\x68\xc7\x30\x7b\xb5\x90\x7a\x46\x94\x82\xcf\x62\x51",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bancor",
        .unit = "BNT",
        .contract_address =
            "\x1f\x57\x3d\x6f\xb3\xf1\x3d\x68\x9f\xf8\x44\xb4\xce\x37\x79\x4d\x79\xa7\xff\x1c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bounty0x",
        .unit = "BNTY",
        .contract_address =
            "\xd2\xd6\x15\x86\x83\xae\xe4\xcc\x83\x80\x67\x72\x72\x09\xa0\xaa\xf4\x35\x9d\xe3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BOSAGORA",
        .unit = "BOA",
        .contract_address =
            "\x74\x6d\xda\x2e\xa2\x43\x40\x0d\x5a\x63\xe0\x70\x0f\x19\x0a\xb7\x9f\x06\x48\x9e",
        .decimals = 7,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BOB Token",
        .unit = "BOB",
        .contract_address =
            "\xDF\x34\x79\x11\x91\x0b\x6c\x9A\x42\x86\xbA\x8E\x2E\xE5\xea\x4a\x39\xeB\x21\x34",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bodhi Ethereum",
        .unit = "BOE",
        .contract_address =
            "\x97\x0e\x03\x5e\x2a\x01\x3c\xf4\xbe\xcd\x67\xe3\x00\xd6\x5b\xc3\x2a\x56\xd8\x26",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Blockium Token",
        .unit = "BOK",
        .contract_address =
            "\x27\xc7\x43\x95\x4b\xce\x1b\xfa\xef\x8b\xcb\xd6\x85\x52\x75\x31\x00\x1d\x88\xd7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bonpay",
        .unit = "BON",
        .contract_address =
            "\xcc\x34\x36\x6e\x38\x42\xca\x1b\xd3\x6c\x1f\x32\x4d\x15\x25\x79\x60\xfc\xc8\x01",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BoutsPro",
        .unit = "BOUTS",
        .contract_address =
            "\x13\x9d\x93\x97\x27\x4b\xb9\xe2\xc2\x9a\x9a\xa8\xaa\x0b\x58\x74\xd3\x0d\x62\xe3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BOX Token",
        .unit = "BOX",
        .contract_address =
            "\xe1\xA1\x78\xB6\x81\xBD\x05\x96\x4d\x3e\x3E\xd3\x3A\xE7\x31\x57\x7d\x9d\x96\xdD",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ContentBox Token",
        .unit = "BOX",
        .contract_address =
            "\x63\xf5\x84\xfa\x56\xe6\x0e\x4d\x0f\xe8\x80\x2b\x27\xc7\xe6\xe3\xb3\x3e\x00\x7f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Boxx",
        .unit = "BOXX",
        .contract_address =
            "\x78\x01\x16\xd9\x1e\x55\x92\xe5\x8a\x3b\x3c\x76\xa3\x51\x57\x1b\x39\xab\xce\xc6",
        .decimals = 15,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Blockport",
        .unit = "BPT",
        .contract_address =
            "\x32\x76\x82\x77\x9b\xab\x2b\xf4\xd1\x33\x7e\x89\x74\xab\x9d\xe8\x27\x5a\x7c\xa8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BqtX Token",
        .unit = "BQTX",
        .contract_address =
            "\x9d\x8b\xe9\x4d\x06\x12\x17\x0c\xe5\x33\xac\x4d\x7b\x43\xcc\x3c\xd9\x1e\x5a\x1a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BRAT",
        .unit = "BRAT",
        .contract_address =
            "\x9e\x77\xd5\xa1\x25\x1b\x6f\x7d\x45\x67\x22\xa6\xea\xc6\xd2\xd5\x98\x0b\xd8\x91",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bread",
        .unit = "BRD",
        .contract_address =
            "\x55\x8e\xc3\x15\x2e\x2e\xb2\x17\x49\x05\xcd\x19\xae\xa4\xe3\x4a\x23\xde\x9a\xd6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bitibu",
        .unit = "BTB",
        .contract_address =
            "\xc4\x29\xe7\xb6\x3d\xc3\x0b\x51\xf5\xa7\xe4\xbe\x23\xfd\x5e\xb4\xba\xf8\xa1\xc3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitAir",
        .unit = "BTCA",
        .contract_address =
            "\x02\x72\x58\x36\xeb\xf3\xec\xdb\x1c\xdf\x1c\x7b\x02\xfc\xbb\xfa\xa2\x73\x6a\xf8",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BTC LITE",
        .unit = "BTCL",
        .contract_address =
            "\x5a\xcd\x19\xb9\xc9\x1e\x59\x6b\x1f\x06\x2f\x18\xe3\xd0\x2d\xa7\xed\x8d\x1e\x50",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitCoin ONE",
        .unit = "BTCONE",
        .contract_address =
            "\x87\xf5\xe8\xc3\x42\x52\x18\x83\x7f\x3c\xb6\x7d\xb9\x41\xaf\x0c\x01\x32\x3e\x56",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitcoinToken",
        .unit = "BTK",
        .contract_address =
            "\xdb\x86\x46\xf5\xb4\x87\xb5\xdd\x97\x9f\xac\x61\x83\x50\xe8\x50\x18\xf5\x57\xd4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bytom",
        .unit = "BTM",
        .contract_address =
            "\xcb\x97\xe6\x5f\x07\xda\x24\xd4\x6b\xcd\xd0\x78\xeb\xeb\xd7\xc6\xe6\xe3\xd7\x50",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitMax token",
        .unit = "BTMX",
        .contract_address =
            "\xf4\x5f\x0e\x16\xb5\xe0\x96\x28\x6e\x1f\xb4\x63\xd3\x4b\xe9\xf3\xdf\x5e\x36\x02",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BTOCoin",
        .unit = "BTO",
        .contract_address =
            "\x36\x90\x5f\xc9\x32\x80\xf5\x23\x62\xa1\xcb\xab\x15\x1f\x25\xdc\x46\x74\x2f\xb5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bitether",
        .unit = "BTR",
        .contract_address =
            "\x49\x9a\x6b\x77\xbc\x25\xc2\x6b\xcf\x82\x65\xe2\x10\x2b\x1b\x3d\xd1\x61\x70\x24",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BTRN",
        .unit = "BTRN",
        .contract_address =
            "\x03\xc7\x80\xcd\x55\x45\x98\x59\x2b\x97\xb7\x25\x6d\xda\xad\x75\x99\x45\xb1\x25",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "blocktrade",
        .unit = "BTT",
        .contract_address =
            "\xfa\x45\x6c\xf5\x52\x50\xa8\x39\x08\x8b\x27\xee\x32\xa4\x24\xd7\xda\xcb\x54\xff",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BTU Protocol",
        .unit = "BTU",
        .contract_address =
            "\xb6\x83\xd8\x3a\x53\x2e\x2c\xb7\xdf\xa5\x27\x5e\xed\x36\x98\x43\x63\x71\xcc\x9f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bulleon",
        .unit = "BUL",
        .contract_address =
            "\x07\x75\xc8\x1a\x27\x3b\x35\x5e\x6a\x5b\x76\xe2\x40\xbf\x70\x87\x01\xf0\x02\x79",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Binance USD",
        .unit = "BUSD",
        .contract_address =
            "\x4f\xab\xb1\x45\xd6\x46\x52\xa9\x48\xd7\x25\x33\x02\x3f\x6e\x7a\x62\x3c\x7c\x53",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BZ",
        .unit = "BZ",
        .contract_address =
            "\x43\x75\xe7\xad\x8a\x01\xb8\xec\x3e\xd0\x41\x39\x9f\x62\xd9\xcd\x12\x0e\x00\x63",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BezantToken",
        .unit = "BZNT",
        .contract_address =
            "\xe1\xae\xe9\x84\x95\x36\x5f\xc1\x79\x69\x9c\x1b\xb3\xe7\x61\xfa\x71\x6b\xee\x62",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bezop",
        .unit = "Bez",
        .contract_address =
            "\x8a\x1e\x39\x30\xfd\xe1\xf1\x51\x47\x1c\x36\x8f\xdb\xb3\x9f\x3f\x63\xa6\x5b\x55",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Crypto20",
        .unit = "C20",
        .contract_address =
            "\x26\xe7\x53\x07\xfc\x0c\x02\x14\x72\xfe\xb8\xf7\x27\x83\x95\x31\xf1\x12\xf3\x17",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Carboneum",
        .unit = "C8",
        .contract_address =
            "\xd4\x2d\xeb\xe4\xed\xc9\x2b\xd5\xa3\xfb\xb4\x24\x3e\x1e\xcc\xf6\xd6\x3a\x4a\x5d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ChangeBank",
        .unit = "CAG",
        .contract_address =
            "\x7d\x4b\x8c\xce\x05\x91\xc9\x04\x4a\x22\xee\x54\x35\x33\xb7\x2e\x97\x6e\x36\xc3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cajutel",
        .unit = "CAJ",
        .contract_address =
            "\x3c\x6a\x7a\xb4\x7b\x5f\x05\x8b\xe0\xe7\xc7\xfe\x1a\x4b\x79\x25\xb8\xac\xa4\x0e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CanYaCoin",
        .unit = "CAN",
        .contract_address =
            "\x1d\x46\x24\x14\xfe\x14\xcf\x48\x9c\x7a\x21\xca\xc7\x85\x09\xf4\xbf\x8c\xd7\xc0",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cappasity",
        .unit = "CAPP",
        .contract_address =
            "\x04\xf2\xe7\x22\x1f\xdb\x1b\x52\xa6\x81\x69\xb2\x57\x93\xe5\x14\x78\xff\x03\x29",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cardstack",
        .unit = "CARD",
        .contract_address =
            "\x95\x4b\x89\x07\x04\x69\x3a\xf2\x42\x61\x3e\xde\xf1\xb6\x03\x82\x5a\xfc\xd7\x08",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cashaa",
        .unit = "CAS",
        .contract_address =
            "\xe8\x78\x0b\x48\xbd\xb0\x5f\x92\x86\x97\xa5\xe8\x15\x5f\x67\x2e\xd9\x14\x62\xf7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BlockCAT",
        .unit = "CAT",
        .contract_address =
            "\x56\xba\x2e\xe7\x89\x04\x61\xf4\x63\xf7\xbe\x02\xaa\xc3\x09\x9f\x6d\x58\x11\xa8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitClave",
        .unit = "CAT",
        .contract_address =
            "\x12\x34\x56\x74\x61\xd3\xf8\xdb\x74\x96\x58\x17\x74\xbd\x86\x9c\x83\xd5\x1c\x93",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CashBetCoin",
        .unit = "CBC",
        .contract_address =
            "\x26\xdb\x54\x39\xf6\x51\xca\xf4\x91\xa8\x7d\x48\x79\x9d\xa8\x1f\x19\x1b\xdb\x6b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CommerceBlock",
        .unit = "CBT",
        .contract_address =
            "\x07\x6C\x97\xe1\xc8\x69\x07\x2e\xE2\x2f\x8c\x91\x97\x8C\x99\xB4\xbc\xB0\x25\x91",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Coinchase Token",
        .unit = "CCH",
        .contract_address =
            "\x40\xad\xfc\x7c\x23\xc2\x2c\xc0\x6f\x94\xf1\x99\xa4\x75\x0d\x71\x96\xf4\x6f\xbe",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ccore",
        .unit = "CCO",
        .contract_address =
            "\x67\x9b\xad\xc5\x51\x62\x6e\x01\xb2\x3c\xee\xce\xfb\xc9\xb8\x77\xea\x18\xfc\x46",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CCRB",
        .unit = "CCRB",
        .contract_address =
            "\xe4\xc9\x4d\x45\xf7\xae\xf7\x01\x8a\x5d\x66\xf4\x4a\xf7\x80\xec\x60\x23\x37\x8e",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Crystal Clear Token",
        .unit = "CCT",
        .contract_address =
            "\x33\x6f\x64\x6f\x87\xd9\xf6\xbc\x6e\xd4\x2d\xd4\x6e\x8b\x3f\xd9\xdb\xd1\x5c\x22",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CoinDash",
        .unit = "CDT",
        .contract_address =
            "\x17\x7d\x39\xac\x67\x6e\xd1\xc6\x7a\x2b\x26\x8a\xd7\xf1\xe5\x88\x26\xe5\xb0\xaf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Network",
        .unit = "CDX",
        .contract_address =
            "\x6f\xff\x38\x06\xbb\xac\x52\xa2\x0e\x0d\x79\xbc\x53\x8d\x52\x7f\x6a\x22\xc9\x6b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CEEK",
        .unit = "CEEK",
        .contract_address =
            "\xb0\x56\xc3\x8f\x6b\x7d\xc4\x06\x43\x67\x40\x3e\x26\x42\x4c\xd2\xc6\x06\x55\xe1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CelerToken",
        .unit = "CELR",
        .contract_address =
            "\x4f\x92\x54\xc8\x3e\xb5\x25\xf9\xfc\xf3\x46\x49\x0b\xbb\x3e\xd2\x8a\x81\xc6\x67",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CEN",
        .unit = "CEN",
        .contract_address =
            "\x0b\xc6\x1d\xde\xd5\xf6\x71\x0c\x63\x7c\xf8\x28\x8e\xb6\x05\x87\x66\xce\x19\x21",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Centrality Token",
        .unit = "CENNZ",
        .contract_address =
            "\x11\x22\xb6\xa0\xe0\x0d\xce\x05\x63\x08\x2b\x6e\x29\x53\xf3\xa9\x43\x85\x5c\x1f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CoinEx Token",
        .unit = "CET",
        .contract_address =
            "\x08\x1f\x67\xaf\xa0\xcc\xf8\xc7\xb1\x75\x40\x76\x7b\xbe\x95\xdf\x2b\xa8\xd9\x7f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cofoundit",
        .unit = "CFI",
        .contract_address =
            "\x12\xfe\xf5\xe5\x7b\xf4\x58\x73\xcd\x9b\x62\xe9\xdb\xd7\xbf\xb9\x9e\x32\xd7\x3e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Poker Chips",
        .unit = "CHP",
        .contract_address =
            "\xf3\xdb\x75\x60\xe8\x20\x83\x46\x58\xb5\x90\xc9\x62\x34\xc3\x33\xcd\x3d\x5e\x5e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Chroma",
        .unit = "CHR",
        .contract_address =
            "\x91\x50\x44\x52\x67\x58\x53\x3d\xfb\x91\x8e\xce\xb6\xe4\x4b\xc2\x16\x32\x06\x0d",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SwissBorg",
        .unit = "CHSB",
        .contract_address =
            "\xba\x9d\x41\x99\xfa\xb4\xf2\x6e\xfe\x35\x51\xd4\x90\xe3\x82\x14\x86\xf1\x35\xba",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Own",
        .unit = "CHX",
        .contract_address =
            "\x14\x60\xa5\x80\x96\xd8\x0a\x50\xa2\xf1\xf9\x56\xdd\xa4\x97\x61\x1f\xa4\xf1\x65",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "chiliZ",
        .unit = "CHZ",
        .contract_address =
            "\x35\x06\x42\x4f\x91\xfd\x33\x08\x44\x66\xf4\x02\xd5\xd9\x7f\x05\xf8\xe3\xb4\xaf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cryptoindex 100",
        .unit = "CIX100",
        .contract_address =
            "\x63\x93\xe8\x22\x87\x47\x28\xf8\xaf\xa7\xe1\xc9\x94\x4e\x41\x7d\x37\xca\x58\x78",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ConnectJob",
        .unit = "CJT",
        .contract_address =
            "\x3a\xbd\xff\x32\xf7\x6b\x42\xe7\x63\x5b\xdb\x7e\x42\x5f\x02\x31\xa5\xf3\xab\x17",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Coinlancer",
        .unit = "CL",
        .contract_address =
            "\xe8\x1d\x72\xd1\x4b\x15\x16\xe6\x8a\xc3\x19\x0a\x46\xc9\x33\x02\xcc\x8e\xd6\x0f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Colu Local Network",
        .unit = "CLN",
        .contract_address =
            "\x41\x62\x17\x8b\x78\xd6\x98\x54\x80\xa3\x08\xb2\x19\x0e\xe5\x51\x74\x60\x40\x6d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Crowd Machine Compute Token",
        .unit = "CMCT",
        .contract_address =
            "\x47\xbc\x01\x59\x77\x98\xdc\xd7\x50\x6d\xcc\xa3\x6a\xc4\x30\x2f\xc9\x3a\x8c\xfb",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CyberMovieChain",
        .unit = "CMCT",
        .contract_address =
            "\x7a\xbc\x60\xb3\x29\x0f\x68\xc8\x5f\x49\x5f\xd2\xe0\xc3\xbd\x27\x88\x37\xa3\x13",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CyberMiles",
        .unit = "CMT",
        .contract_address =
            "\xf8\x5f\xee\xa2\xfd\xd8\x1d\x51\x17\x7f\x6b\x8f\x35\xf0\xe6\x73\x4c\xe4\x5f\x5f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CONNECT",
        .unit = "CNCT",
        .contract_address =
            "\x54\xa9\xed\x32\x7f\x26\x14\x31\x69\x14\xc3\xf3\xa7\x82\xa7\x7d\x0a\xa4\x7a\xee",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cindicator",
        .unit = "CND",
        .contract_address =
            "\xd4\xc4\x35\xf5\xb0\x9f\x85\x5c\x33\x17\xc8\x52\x4c\xb1\xf5\x86\xe4\x27\x95\xfa",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CNN Token",
        .unit = "CNN",
        .contract_address =
            "\x87\x13\xd2\x66\x37\xcf\x49\xe1\xb6\xb4\xa7\xce\x57\x10\x6a\xab\xc9\x32\x53\x43",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CoinUs",
        .unit = "CNUS",
        .contract_address =
            "\x72\x2f\x2f\x3e\xac\x7e\x95\x97\xc7\x3a\x59\x3f\x7c\xf3\xde\x33\xfb\xfc\x33\x08",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cobinhood",
        .unit = "COB",
        .contract_address =
            "\xb2\xf7\xeb\x1f\x2c\x37\x64\x5b\xe6\x1d\x73\x95\x30\x35\x36\x0e\x76\x8d\x81\xe6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CocosToken",
        .unit = "COCOS",
        .contract_address =
            "\x0c\x6f\x5f\x7d\x55\x5e\x75\x18\xf6\x84\x1a\x79\x43\x6b\xd2\xb1\xee\xf0\x33\x81",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CoinFi",
        .unit = "COFI",
        .contract_address =
            "\x31\x36\xef\x85\x15\x92\xac\xf4\x9c\xa4\xc8\x25\x13\x1e\x36\x41\x70\xfa\x32\xb3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Coinvest COIN V3 Token",
        .unit = "COIN",
        .contract_address =
            "\xeb\x54\x7e\xd1\xD8\xA3\xFf\x14\x61\xaB\xAa\x7F\x00\x22\xFE\xD4\x83\x6E\x00\xA4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "COS",
        .unit = "COS",
        .contract_address =
            "\x7d\x3c\xb1\x1f\x8c\x13\x73\x0c\x24\xd0\x18\x26\xd8\xf2\x00\x5f\x0e\x1b\x34\x8f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CosmoCoin",
        .unit = "COSM",
        .contract_address =
            "\xd1\xe1\x0c\x37\xa2\x7d\x95\xd9\x57\x20\x29\x1b\x1d\xc6\xf1\x2f\x74\xc7\x14\x43",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Couchain",
        .unit = "COU",
        .contract_address =
            "\xf0\x91\xcf\x09\xc5\x18\x11\x81\x9d\xb7\x05\x71\x0e\x96\x34\xb8\xbf\x18\xf1\x64",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Covesting",
        .unit = "COV",
        .contract_address =
            "\xe2\xfb\x65\x29\xef\x56\x6a\x08\x0e\x6d\x23\xde\x0b\xd3\x51\x31\x10\x87\xd5\x67",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Covalent Token",
        .unit = "COVA",
        .contract_address =
            "\xb3\x7a\x76\x9b\x37\x22\x44\x49\xd9\x2a\xac\x57\xde\x37\x9e\x12\x67\xcd\x3b\x00",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CPAY",
        .unit = "CPAY",
        .contract_address =
            "\x0E\xbb\x61\x42\x04\xE4\x7c\x09\xB6\xC3\xFe\xB9\xAA\xeC\xad\x8E\xE0\x60\xE2\x3E",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CPChain",
        .unit = "CPC",
        .contract_address =
            "\xfA\xE4\xEe\x59\xCD\xd8\x6e\x3B\xe9\xe8\xb9\x0b\x53\xAA\x86\x63\x27\xD7\xc0\x90",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cryptaur",
        .unit = "CPT",
        .contract_address =
            "\x88\xd5\x0b\x46\x6b\xe5\x52\x22\x01\x9d\x71\xf9\xe8\xfa\xe1\x7f\x5f\x45\xfc\xa1",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "COPYTRACK",
        .unit = "CPY",
        .contract_address =
            "\xf4\x47\x45\xfb\xd4\x1f\x6a\x1b\xa1\x51\xdf\x19\x0d\xb0\x56\x4c\x5f\xcc\x44\x10",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CreditBIT",
        .unit = "CRB",
        .contract_address =
            "\xAe\xf3\x8f\xBF\xBF\x93\x2D\x1A\xeF\x3B\x80\x8B\xc8\xfB\xd8\xCd\x8E\x1f\x8B\xC5",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CryCash",
        .unit = "CRC",
        .contract_address =
            "\xf4\x1e\x5f\xbc\x2f\x6a\xac\x20\x0d\xd8\x61\x9e\x12\x1c\xe1\xf0\x5d\x15\x00\x77",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CryptalDash",
        .unit = "CRD",
        .contract_address =
            "\xca\xaa\x93\x71\x2b\xda\xc3\x7f\x73\x6c\x32\x3c\x93\xd4\xd5\xfd\xef\xcc\x31\xcc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CarryToken",
        .unit = "CRE",
        .contract_address =
            "\x11\x5e\xc7\x9f\x1d\xe5\x67\xec\x68\xb7\xae\x7e\xda\x50\x1b\x40\x66\x26\x47\x8e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Verify",
        .unit = "CRED",
        .contract_address =
            "\x67\x2a\x1a\xd4\xf6\x67\xfb\x18\xa3\x33\xaf\x13\x66\x7a\xa0\xaf\x1f\x5b\x5b\xdd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Credo Token",
        .unit = "CREDO",
        .contract_address =
            "\x4e\x06\x03\xe2\xa2\x7a\x30\x48\x0e\x5e\x3a\x4f\xe5\x48\xe2\x9e\xf1\x2f\x64\xbe",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Crypto.com Coin",
        .unit = "CRO",
        .contract_address =
            "\xa0\xb7\x3e\x1f\xf0\xb8\x09\x14\xab\x6f\xe0\x44\x4e\x65\x84\x8c\x4c\x34\x45\x0b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Crypterium",
        .unit = "CRPT",
        .contract_address =
            "\x80\xa7\xe0\x48\xf3\x7a\x50\x50\x03\x51\xc2\x04\xcb\x40\x77\x66\xfa\x3b\xae\x7f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CREDITS",
        .unit = "CS",
        .contract_address =
            "\x46\xb9\xad\x94\x4d\x10\x59\x45\x0d\xa1\x16\x35\x11\x06\x9c\x71\x8f\x69\x9d\x31",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BitDice",
        .unit = "CSNO",
        .contract_address =
            "\x29\xd7\x52\x77\xac\x7f\x03\x35\xb2\x16\x5d\x08\x95\xe8\x72\x5c\xbf\x65\x8d\x73",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cryptosolartech",
        .unit = "CST",
        .contract_address =
            "\xbb\x49\xa5\x1e\xe5\xa6\x6c\xa3\xa8\xcb\xe5\x29\x37\x9b\xa4\x4b\xa6\x7e\x67\x71",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Coimatic 3.0",
        .unit = "CTIC3",
        .contract_address =
            "\x07\x43\x39\x21\x32\xd1\xa0\x3a\x90\x2c\x47\x7e\x5a\x17\x6f\x25\x6b\xa3\x22\x0c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Centra",
        .unit = "CTR",
        .contract_address =
            "\x96\xA6\x56\x09\xa7\xB8\x4E\x88\x42\x73\x2D\xEB\x08\xf5\x6C\x3E\x21\xaC\x6f\x8a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cryptrust",
        .unit = "CTRT",
        .contract_address =
            "\x86\x06\xa8\xf2\x8e\x1e\x2f\xd5\x0b\x90\x74\xd6\x5c\x01\x54\x8b\x1f\x04\x0b\x32",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CarTaxi",
        .unit = "CTX",
        .contract_address =
            "\x66\x2a\xbc\xad\x0b\x7f\x34\x5a\xb7\xff\xb1\xb1\xfb\xb9\xdf\x78\x94\xf1\x8e\x66",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Cortex Coin",
        .unit = "CTXC",
        .contract_address =
            "\xea\x11\x75\x5a\xe4\x1d\x88\x9c\xee\xc3\x9a\x63\xe6\xff\x75\xa0\x2b\xc1\xc0\x0d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Civic",
        .unit = "CVC",
        .contract_address =
            "\x41\xe5\x56\x00\x54\x82\x4e\xa6\xb0\x73\x2e\x65\x6e\x3a\xd6\x4e\x20\xe9\x4e\x45",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CyberVeinToken",
        .unit = "CVT",
        .contract_address =
            "\xbe\x42\x8c\x38\x67\xf0\x5d\xea\x2a\x89\xfc\x76\xa1\x02\xb5\x44\xea\xc7\xf7\x72",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CWV Chain",
        .unit = "CWV",
        .contract_address =
            "\xed\x49\x4c\x9e\x2f\x8e\x34\xe5\x3b\xdd\x0e\xa9\xb4\xd8\x03\x05\xcb\x15\xc5\xc2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CargoX",
        .unit = "CXO",
        .contract_address =
            "\xb6\xee\x96\x68\x77\x1a\x79\xbe\x79\x67\xee\x29\xa6\x3d\x41\x84\xf8\x09\x71\x43",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CyberFM",
        .unit = "CYFM",
        .contract_address =
            "\x32\xb8\x7f\xb8\x16\x74\xaa\x79\x21\x4e\x51\xae\x42\xd5\x71\x13\x6e\x29\xd3\x85",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CrystalToken",
        .unit = "CYL",
        .contract_address =
            "\x26\xcb\x36\x41\xaa\xa4\x39\x11\xf1\xd4\xcb\x2c\xe5\x44\xeb\x65\x2a\xac\x7c\x47",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CyberMusic",
        .unit = "CYMT",
        .contract_address =
            "\x78\xc2\x92\xd1\x44\x5e\x6b\x95\x58\xbf\x42\xe8\xbc\x36\x92\x71\xde\xd0\x62\xea",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CZR",
        .unit = "CZR",
        .contract_address =
            "\x02\x23\xfc\x70\x57\x42\x14\xf6\x58\x13\xfe\x33\x6d\x87\x0a\xc4\x7e\x14\x7f\xae",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Davinci coin",
        .unit = "DAC",
        .contract_address =
            "\xaa\xd5\x4c\x9f\x27\xb8\x76\xd2\x53\x84\x55\xdd\xa6\x92\x07\x27\x9f\xf6\x73\xa5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DACSEE",
        .unit = "DACS",
        .contract_address =
            "\xa3\x11\x08\xe5\xba\xb5\x49\x45\x60\xdb\x34\xc9\x54\x92\x65\x8a\xf2\x39\x35\x7c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DADI",
        .unit = "DADI",
        .contract_address =
            "\xfb\x2f\x26\xf2\x66\xfb\x28\x05\xa3\x87\x23\x0f\x2a\xa0\xa3\x31\xb4\xd9\x6f\xba",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DAGT Crypto Platform",
        .unit = "DAGT",
        .contract_address =
            "\x56\xD1\xaE\x30\xc9\x72\x88\xDA\x4B\x58\xBC\x39\xF0\x26\x09\x17\x78\xe4\xE3\x16",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dai Stablecoin",
        .unit = "DAI",
        .contract_address =
            "\x6b\x17\x54\x74\xe8\x90\x94\xc4\x4d\xa9\x8b\x95\x4e\xed\xea\xc4\x95\x27\x1d\x0f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DALECOIN",
        .unit = "DALC",
        .contract_address =
            "\x07\xd9\xe4\x9e\xa4\x02\x19\x4b\xf4\x8a\x82\x76\xda\xfb\x16\xe4\xed\x63\x33\x17",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Daneel",
        .unit = "DAN",
        .contract_address =
            "\x9b\x70\x74\x0e\x70\x8a\x08\x3c\x6f\xf3\x8d\xf5\x22\x97\x02\x0f\x5d\xfa\xa5\xee",
        .decimals = 10,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DAPSTOKEN",
        .unit = "DAPS",
        .contract_address =
            "\x93\x19\x0d\xbc\xe9\xb9\xbd\x4a\xa5\x46\x27\x0a\x8d\x1d\x65\x90\x5b\x5f\xdd\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Datum",
        .unit = "DAT",
        .contract_address =
            "\x81\xc9\x15\x1d\xe0\xc8\xba\xfc\xd3\x25\xa5\x7e\x3d\xb5\xa5\xdf\x1c\xeb\xf7\x9c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DATAcoin",
        .unit = "DATA",
        .contract_address =
            "\x0c\xf0\xee\x63\x78\x8a\x08\x49\xfe\x52\x97\xf3\x40\x7f\x70\x1e\x12\x2c\xc0\x23",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DATx",
        .unit = "DATx",
        .contract_address =
            "\xab\xbb\xb6\x44\x7b\x68\xff\xd6\x14\x1d\xa7\x7c\x18\xc7\xb5\x87\x6e\xd6\xc5\xab",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DAV Token",
        .unit = "DAV",
        .contract_address =
            "\xd8\x2d\xf0\xab\xd3\xf5\x14\x25\xeb\x15\xef\x75\x80\xfd\xa5\x57\x27\x87\x5f\x14",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DAEX Token",
        .unit = "DAX",
        .contract_address =
            "\x0b\x4b\xdc\x47\x87\x91\x89\x72\x74\x65\x2d\xc1\x5e\xf5\xc1\x35\xca\xe6\x1e\x60",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DAY",
        .unit = "DAY",
        .contract_address =
            "\xE8\x14\xae\xE9\x60\xa8\x52\x08\xC3\xdB\x54\x2C\x53\xE7\xD4\xa6\xC8\xD5\xf6\x0F",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Decent.Bet",
        .unit = "DBET",
        .contract_address =
            "\x9b\x68\xbf\xae\x21\xdf\x5a\x51\x09\x31\xa2\x62\xce\xcf\x63\xf4\x13\x38\xf2\x64",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Distributed Credit Chain",
        .unit = "DCC",
        .contract_address =
            "\xff\xa9\x3a\xac\xf4\x92\x97\xd5\x1e\x21\x18\x17\x45\x28\x39\x05\x2f\xdf\xb9\x61",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Digital Developers Fund Token",
        .unit = "DDF",
        .contract_address =
            "\xcc\x4e\xf9\xee\xaf\x65\x6a\xc1\xa2\xab\x88\x67\x43\xe9\x8e\x97\xe0\x90\xed\x38",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DEBITUM",
        .unit = "DEB",
        .contract_address =
            "\x15\x12\x02\xc9\xc1\x8e\x49\x56\x56\xf3\x72\x28\x1f\x49\x3e\xb7\x69\x89\x61\xd5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Darico Ecosystem Coin",
        .unit = "DEC",
        .contract_address =
            "\x89\xc6\xc8\x56\xa6\xdb\x3e\x46\x10\x71\x63\xd0\xcd\xa7\xa7\xff\x21\x1b\xd6\x55",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DeltaChain Token",
        .unit = "DELTA",
        .contract_address =
            "\xde\x1e\x0a\xe6\x10\x1b\x46\x52\x0c\xf6\x6f\xdc\x0b\x10\x59\xc5\xcc\x3d\x10\x6c",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DENT",
        .unit = "DENT",
        .contract_address =
            "\x35\x97\xbf\xd5\x33\xa9\x9c\x9a\xa0\x83\x58\x7b\x07\x44\x34\xe6\x1e\xb0\xa2\x58",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DEW",
        .unit = "DEW",
        .contract_address =
            "\x20\xe9\x48\x67\x79\x4d\xba\x03\x0e\xe2\x87\xf1\x40\x6e\x10\x0d\x03\xc8\x4c\xd3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DGD",
        .unit = "DGD",
        .contract_address =
            "\xe0\xb7\x92\x7c\x4a\xf2\x37\x65\xcb\x51\x31\x4a\x0e\x05\x21\xa9\x64\x5f\x0e\x2a",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DigiPulse",
        .unit = "DGPT",
        .contract_address =
            "\xf6\xcf\xe5\x3d\x6f\xeb\xae\xea\x05\x1f\x40\x0f\xf5\xfc\x14\xf0\xcb\xbd\xac\xa1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dragonglass",
        .unit = "DGS",
        .contract_address =
            "\x6a\xed\xbf\x8d\xff\x31\x43\x72\x20\xdf\x35\x19\x50\xba\x2a\x33\x62\x16\x8d\x1b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DigitexFutures",
        .unit = "DGTX",
        .contract_address =
            "\x1c\x83\x50\x14\x78\xf1\x32\x09\x77\x04\x70\x08\x49\x6d\xac\xbd\x60\xbb\x15\xef",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Digix Gold Token",
        .unit = "DGX",
        .contract_address =
            "\x4f\x3a\xfe\xc4\xe5\xa3\xf2\xa6\xa1\xa4\x11\xde\xf7\xd7\xdf\xe5\x0e\xe0\x57\xbf",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Inmediate",
        .unit = "DIT",
        .contract_address =
            "\xf1\x49\x22\x00\x1a\x2f\xb8\x54\x1a\x43\x39\x05\x43\x7a\xe9\x54\x41\x9c\x24\x39",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Divi Exchange Token",
        .unit = "DIVX",
        .contract_address =
            "\x13\xf1\x1c\x99\x05\xa0\x8c\xa7\x6e\x3e\x85\x3b\xe6\x3d\x4f\x09\x44\x32\x6c\x72",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Delta",
        .unit = "DLT",
        .contract_address =
            "\x07\xe3\xc7\x06\x53\x54\x8b\x04\xf0\xa7\x59\x70\xc1\xf8\x1b\x4c\xbb\xfb\x60\x6f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DMarket",
        .unit = "DMT",
        .contract_address =
            "\x2c\xcb\xff\x3a\x04\x2c\x68\x71\x6e\xd2\xa2\xcb\x0c\x54\x4a\x9f\x1d\x19\x35\xe1",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DNA",
        .unit = "DNA",
        .contract_address =
            "\x82\xb0\xe5\x04\x78\xee\xaf\xde\x39\x2d\x45\xd1\x25\x9e\xd1\x07\x1b\x6f\xda\x81",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "district0x",
        .unit = "DNT",
        .contract_address =
            "\x0a\xbd\xac\xe7\x0d\x37\x90\x23\x5a\xf4\x48\xc8\x85\x47\x60\x3b\x94\x56\x04\xea",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dock",
        .unit = "DOCK",
        .contract_address =
            "\xe5\xda\xda\x80\xaa\x64\x77\xe8\x5d\x09\x74\x7f\x28\x42\xf7\x99\x3d\x0d\xf7\x1c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DoradoToken",
        .unit = "DOR",
        .contract_address =
            "\x90\x6b\x3f\x8b\x78\x45\x84\x01\x88\xea\xb5\x3c\x3f\x5a\xd3\x48\xa7\x87\x75\x2f",
        .decimals = 15,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DOS Network Token",
        .unit = "DOS",
        .contract_address =
            "\x70\x86\x1e\x86\x2e\x1a\xc0\xc9\x6f\x85\x3c\x82\x31\x82\x6e\x46\x9e\xad\x37\xb1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DOVU",
        .unit = "DOV",
        .contract_address =
            "\xac\x32\x11\xa5\x02\x54\x14\xaf\x28\x66\xff\x09\xc2\x3f\xc1\x8b\xc9\x7e\x79\xb1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DA Power Play",
        .unit = "DPP",
        .contract_address =
            "\x01\xb3\xec\x4a\xae\x1b\x87\x29\x52\x9b\xeb\x49\x65\xf2\x7d\x00\x87\x88\xb0\xeb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Delphy Token",
        .unit = "DPY",
        .contract_address =
            "\x6c\x2a\xdc\x20\x73\x99\x4f\xb2\xcc\xc5\x03\x2c\xc2\x90\x6f\xa2\x21\xe9\xb3\x91",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DRAGON",
        .unit = "DRG",
        .contract_address =
            "\x81\x4f\x67\xfa\x28\x6f\x75\x72\xb0\x41\xd0\x41\xb1\xd9\x9b\x43\x2c\x91\x55\xee",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dragon",
        .unit = "DRGN",
        .contract_address =
            "\x41\x9c\x4d\xb4\xb9\xe2\x5d\x6d\xb2\xad\x96\x91\xcc\xb8\x32\xc8\xd9\xfd\xa0\x5e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dropil",
        .unit = "DROP",
        .contract_address =
            "\x46\x72\xba\xd5\x27\x10\x74\x71\xcb\x50\x67\xa8\x87\xf4\x65\x6d\x58\x5a\x8a\x31",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DCORP",
        .unit = "DRP",
        .contract_address =
            "\x62\x1d\x78\xf2\xef\x2f\xd9\x37\xbf\xca\x69\x6c\xab\xaf\x9a\x77\x9f\x59\xb3\xed",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DRP Utility",
        .unit = "DRPU",
        .contract_address =
            "\xe3\x0e\x02\xf0\x49\x95\x7e\x2a\x59\x07\x58\x9e\x06\xba\x64\x6f\xb2\xc3\x21\xba",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DomRaiderToken",
        .unit = "DRT",
        .contract_address =
            "\x9a\xf4\xf2\x69\x41\x67\x7c\x70\x6c\xfe\xcf\x6d\x33\x79\xff\x01\xbb\x85\xd5\xab",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dether",
        .unit = "DTH",
        .contract_address =
            "\x5a\xdc\x96\x1d\x6a\xc3\xf7\x06\x2d\x2e\xa4\x5f\xef\xb8\xd8\x16\x7d\x44\xb1\x90",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Datarius Credit",
        .unit = "DTRC",
        .contract_address =
            "\xc2\x04\x64\xe0\xc3\x73\x48\x6d\x2b\x33\x35\x57\x6e\x83\xa2\x18\xb1\x61\x8a\x5e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DaTa eXchange Token",
        .unit = "DTX",
        .contract_address =
            "\x76\x5f\x0c\x16\xd1\xdd\xc2\x79\x29\x5c\x1a\x7c\x24\xb0\x88\x3f\x62\xd3\x3f\x75",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dusk Network",
        .unit = "DUSK",
        .contract_address =
            "\x94\x0a\x2d\xb1\xb7\x00\x8b\x6c\x77\x6d\x4f\xaa\xca\x72\x9d\x6d\x4a\x4a\xa5\x51",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DxChain Token",
        .unit = "DX",
        .contract_address =
            "\x97\x3e\x52\x69\x11\x76\xd3\x64\x53\x86\x8d\x9d\x86\x57\x27\x88\xd2\x70\x41\xa9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DataWallet",
        .unit = "DXT",
        .contract_address =
            "\x8d\xb5\x4c\xa5\x69\xd3\x01\x9a\x2b\xa1\x26\xd0\x3c\x37\xc4\x4b\x5e\xf8\x1e\xf6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Dentacoin",
        .unit = "Dentacoin",
        .contract_address =
            "\x08\xd3\x2b\x0d\xa6\x3e\x2C\x3b\xcF\x80\x19\xc9\xc5\xd8\x49\xd7\xa9\xd7\x91\xe6",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "E4ROW",
        .unit = "E4ROW",
        .contract_address =
            "\xce\x5c\x60\x3c\x78\xd0\x47\xef\x43\x03\x2e\x96\xb5\xb7\x85\x32\x4f\x75\x3a\x4f",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EagleCoin",
        .unit = "EAGLE",
        .contract_address =
            "\x99\x4f\x0d\xff\xdb\xae\x0b\xbf\x09\xb6\x52\xd6\xf1\x1a\x49\x3f\xd3\x3f\x42\xb9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EARTH Token",
        .unit = "EARTH",
        .contract_address =
            "\x90\x0b\x44\x49\x23\x6a\x7b\xb2\x6b\x28\x66\x01\xdd\x14\xd2\xbd\xe7\xa6\xac\x6c",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EBCoin",
        .unit = "EBC",
        .contract_address =
            "\x31\xf3\xd9\xd1\xbe\xce\x0c\x03\x3f\xf7\x8f\xa6\xda\x60\xa6\x04\x8f\x3e\x13\xc5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethbet",
        .unit = "EBET",
        .contract_address =
            "\x7D\x5E\xdc\xd2\x3d\xAa\x3f\xB9\x43\x17\xD3\x2a\xE2\x53\xeE\x1A\xf0\x8B\xa1\x4d",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "eBitcoin",
        .unit = "EBTC",
        .contract_address =
            "\xeb\x7c\x20\x02\x71\x72\xe5\xd1\x43\xfb\x03\x0d\x50\xf9\x1c\xec\xe2\xd1\x48\x5d",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Omnitude",
        .unit = "ECOM",
        .contract_address =
            "\x17\x1d\x75\x0d\x42\xd6\x61\xb6\x2c\x27\x7a\x6b\x48\x6a\xdb\x82\x34\x8c\x3e\xca",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EcoRealEstate",
        .unit = "ECOREAL",
        .contract_address =
            "\xb0\x52\xf8\xa3\x3d\x8b\xb0\x68\x41\x4e\xad\xe0\x6a\xf6\x95\x51\x99\xf9\xf0\x10",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Edgeless",
        .unit = "EDG",
        .contract_address =
            "\x08\x71\x1d\x3b\x02\xc8\x75\x8f\x2f\xb3\xab\x4e\x80\x22\x84\x18\xa7\xf8\xe3\x9c",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Eden Coin",
        .unit = "EDN",
        .contract_address =
            "\x05\x86\x0d\x45\x3c\x79\x74\xcb\xf4\x65\x08\xc0\x6c\xba\x14\xe2\x11\xc6\x29\xce",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Eidoo",
        .unit = "EDO",
        .contract_address =
            "\xce\xd4\xe9\x31\x98\x73\x4d\xda\xff\x84\x92\xd5\x25\xbd\x25\x8d\x49\xeb\x38\x8e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Endor Protocol Token",
        .unit = "EDR",
        .contract_address =
            "\xc5\x28\xc2\x8f\xec\x0a\x90\xc0\x83\x32\x8b\xc4\x5f\x58\x7e\xe2\x15\x76\x0a\x0f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Egretia",
        .unit = "EGT",
        .contract_address =
            "\x8e\x1b\x44\x8e\xc7\xad\xfc\x7f\xa3\x5f\xc2\xe8\x85\x67\x8b\xd3\x23\x17\x6e\x34",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EngagementToken",
        .unit = "EGT",
        .contract_address =
            "\x5d\xba\xc2\x4e\x98\xe2\xa4\xf4\x3a\xdc\x0d\xc8\x2a\xf4\x03\xfc\xa0\x63\xce\x2c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EchoLink",
        .unit = "EKO",
        .contract_address =
            "\xa6\xa8\x40\xe5\x0b\xca\xa5\x0d\xa0\x17\xb9\x1a\x0d\x86\xb8\xb2\xd4\x11\x56\xee",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EKT",
        .unit = "EKT",
        .contract_address =
            "\x4e\xcd\xb6\x38\x5f\x3d\xb3\x84\x7f\x9c\x4a\x9b\xf3\xf9\x91\x7b\xb2\x7a\x54\x52",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ElacToken",
        .unit = "ELAC",
        .contract_address =
            "\xec\x6c\x86\x1c\x2a\x2b\x1e\x5f\xf5\x33\x67\x31\xbc\x80\xc2\x9d\xbf\xf8\x82\x73",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ElectrifyAsia",
        .unit = "ELEC",
        .contract_address =
            "\xd4\x9f\xf1\x36\x61\x45\x13\x13\xca\x15\x53\xfd\x69\x54\xbd\x1d\x9b\x6e\x02\xb9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Elementeum",
        .unit = "ELET",
        .contract_address =
            "\x6c\x37\xbf\x4f\x04\x27\x12\xc9\x78\xa7\x3e\x3f\xd5\x6d\x1f\x57\x38\xdd\x7c\x43",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ELF",
        .unit = "ELF",
        .contract_address =
            "\xbf\x21\x79\x85\x9f\xc6\xd5\xbe\xe9\xbf\x91\x58\x63\x2d\xc5\x16\x78\xa4\x10\x0e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EligmaToken",
        .unit = "ELI",
        .contract_address =
            "\xc7\xc0\x3b\x8a\x3f\xc5\x71\x90\x66\xe1\x85\xea\x61\x6e\x87\xb8\x8e\xba\x44\xa3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ELTCOIN",
        .unit = "ELTCOIN",
        .contract_address =
            "\x44\x19\x7a\x4c\x44\xd6\xa0\x59\x29\x7c\xaf\x6b\xe4\xf7\xe1\x72\xbd\x56\xca\xaf",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Elycoin",
        .unit = "ELY",
        .contract_address =
            "\xa9\x55\x92\xdc\xff\xa3\xc0\x80\xb4\xb4\x0e\x45\x9c\x5f\x56\x92\xf6\x7d\xb7\xf8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Embers",
        .unit = "EMB",
        .contract_address =
            "\x38\x64\x67\xf1\xf3\xdd\xbe\x83\x24\x48\x65\x04\x18\x31\x1a\x47\x9e\xec\xfc\x57",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EthereumMovieVenture",
        .unit = "EMV",
        .contract_address =
            "\xb8\x02\xb2\x4e\x06\x37\xc2\xb8\x7d\x2e\x8b\x77\x84\xc0\x55\xbb\xe9\x21\x01\x1a",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Enigma",
        .unit = "ENG",
        .contract_address =
            "\xf0\xee\x6b\x27\xb7\x59\xc9\x89\x3c\xe4\xf0\x94\xb4\x9a\xd2\x8f\xd1\x5a\x23\xe4",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EnjinCoin",
        .unit = "ENJ",
        .contract_address =
            "\xf6\x29\xcb\xd9\x4d\x37\x91\xc9\x25\x01\x52\xbd\x8d\xfb\xdf\x38\x0e\x2a\x3b\x9c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EmphyCoin",
        .unit = "EPY",
        .contract_address =
            "\x50\xee\x67\x46\x89\xd7\x5c\x0f\x88\xe8\xf8\x3c\xfe\x8c\x4b\x69\xe8\xfd\x59\x0d",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ERC20",
        .unit = "ERC20",
        .contract_address =
            "\xc3\x76\x1e\xb9\x17\xcd\x79\x0b\x30\xda\xd9\x9f\x6c\xc5\xb4\xff\x93\xc4\xf9\xea",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EROSCOIN",
        .unit = "ERO",
        .contract_address =
            "\x74\xce\xda\x77\x28\x1b\x33\x91\x42\xa3\x68\x17\xfa\x5f\x9e\x29\x41\x2b\xab\x85",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Eristica TOKEN",
        .unit = "ERT",
        .contract_address =
            "\x92\xA5\xB0\x4D\x0E\xD5\xD9\x4D\x7a\x19\x3d\x1d\x33\x4D\x3D\x16\x99\x6f\x4E\x13",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ESSENTIA",
        .unit = "ESS",
        .contract_address =
            "\xfc\x05\x98\x7b\xd2\xbe\x48\x9a\xcc\xf0\xf5\x09\xe4\x4b\x01\x45\xd6\x82\x40\xf7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ESZCoin",
        .unit = "ESZ",
        .contract_address =
            "\xe8\xa1\xdf\x95\x8b\xe3\x79\x04\x5e\x2b\x46\xa3\x1a\x98\xb9\x3a\x2e\xcd\xfd\xed",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ETHEERA",
        .unit = "ETA",
        .contract_address =
            "\x91\x95\xe0\x04\x02\xab\xe3\x85\xf2\xd0\x0a\x32\xaf\x40\xb2\x71\xf2\xe8\x79\x25",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EthBits ETBS Token",
        .unit = "ETBS",
        .contract_address =
            "\x1b\x97\x43\xf5\x56\xd6\x5e\x75\x7c\x4c\x65\x0b\x45\x55\xba\xf3\x54\xcb\x8b\xd3",
        .decimals = 12,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethereum Gold",
        .unit = "ETG",
        .contract_address =
            "\x28\xc8\xd0\x1f\xf6\x33\xea\x9c\xd8\xfc\x6a\x45\x1d\x74\x57\x88\x9e\x69\x8d\xe6",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethereum Dark",
        .unit = "ETHD",
        .contract_address =
            "\xdb\xfb\x42\x3e\x9b\xbf\x16\x29\x43\x88\xe0\x76\x96\xa5\x12\x0e\x4c\xeb\xa0\xc5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethereum Meta",
        .unit = "ETHM",
        .contract_address =
            "\xd5\xda\xd1\xdb\x7f\x11\x20\x37\xc0\xc6\xcf\x07\x92\xdc\x2a\x78\x66\xbf\xd1\x36",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethos",
        .unit = "ETHOS",
        .contract_address =
            "\x5a\xf2\xbe\x19\x3a\x6a\xbc\xa9\xc8\x81\x70\x01\xf4\x57\x44\x77\x7d\xb3\x07\x56",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EnergiToken",
        .unit = "ETK",
        .contract_address =
            "\x3c\x4a\x3f\xfd\x81\x3a\x10\x7f\xeb\xd5\x7b\x2f\x01\xbc\x34\x42\x64\xd9\x0f\xde",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "STASIS EURS Token",
        .unit = "EURS",
        .contract_address =
            "\xdb\x25\xf2\x11\xab\x05\xb1\xc9\x7d\x59\x55\x16\xf4\x57\x94\x52\x8a\x80\x7a\xd8",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EventChain",
        .unit = "EVC",
        .contract_address =
            "\xb6\x2d\x18\xde\xa7\x40\x45\xe8\x22\x35\x2c\xe4\xb3\xee\x77\x31\x9d\xc5\xff\x2f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Devery.io",
        .unit = "EVE",
        .contract_address =
            "\x92\x31\x08\xa4\x39\xc4\xe8\xc2\x31\x5c\x4f\x65\x21\xe5\xce\x95\xb4\x4e\x9b\x4c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Envion",
        .unit = "EVN",
        .contract_address =
            "\xd7\x80\xae\x2b\xf0\x4c\xd9\x6e\x57\x7d\x3d\x01\x47\x62\xf8\x31\xd9\x71\x29\xd0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EvenCoin",
        .unit = "EVN",
        .contract_address =
            "\x68\x90\x9e\x58\x6e\xea\xc8\xf4\x73\x15\xe8\x4b\x4c\x97\x88\xdd\x54\xef\x65\xbb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Everex",
        .unit = "EVX",
        .contract_address =
            "\xf3\xdb\x5f\xa2\xc6\x6b\x7a\xf3\xeb\x0c\x0b\x78\x25\x10\x81\x6c\xbe\x48\x13\xb8",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Eximchain Token",
        .unit = "EXC",
        .contract_address =
            "\x00\xc4\xb3\x98\x50\x06\x45\xeb\x5d\xa0\x0a\x1a\x37\x9a\x88\xb1\x16\x83\xba\x01",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EXMR FDN.",
        .unit = "EXMR",
        .contract_address =
            "\x33\x1f\xA6\xC9\x7c\x64\xe4\x74\x75\x16\x4b\x9f\xC8\x14\x3b\x53\x3c\x5e\xF5\x29",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EXRP Network",
        .unit = "EXRN",
        .contract_address =
            "\xe4\x69\xc4\x47\x3a\xf8\x22\x17\xb3\x0c\xf1\x7b\x10\xbc\xdb\x6c\x8c\x79\x6e\x75",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Experty",
        .unit = "EXY",
        .contract_address =
            "\x5c\x74\x3a\x35\xE9\x03\xF6\xc5\x84\x51\x4e\xc6\x17\xAC\xEe\x06\x11\xCf\x44\xf3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EZToken",
        .unit = "EZT",
        .contract_address =
            "\x5e\x60\x16\xae\x7d\x7c\x49\xd3\x47\xdc\xf8\x34\x86\x0b\x9f\x3e\xe2\x82\x81\x2b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EZOOW",
        .unit = "EZW",
        .contract_address =
            "\x78\xa2\xa1\x02\x9e\x31\x68\xb4\x9d\x3a\x27\x6c\x78\x70\x50\xff\x51\x06\xdc\xf2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Faceter",
        .unit = "FACE",
        .contract_address =
            "\x1c\xca\xa0\xf2\xa7\x21\x0d\x76\xe1\xfd\xec\x74\x0d\x5f\x32\x3e\x2e\x1b\x16\x72",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FaithCoin",
        .unit = "FAITH",
        .contract_address =
            "\xe5\x31\x64\x2e\x9b\xb5\xd0\x27\xe9\xc2\x0e\x03\x28\x42\x87\xb9\x79\x19\xa9\xa5",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FiveBalance",
        .unit = "FBN",
        .contract_address =
            "\x56\x8f\x9f\x32\x96\x9e\x29\xb5\xce\x1a\x45\x45\xbe\x53\x98\xe8\xcc\x7c\x44\x01",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Friendz Coin",
        .unit = "FDZ",
        .contract_address =
            "\x23\x35\x20\x36\xe9\x11\xa2\x2c\xfc\x69\x2b\x5e\x2e\x19\x66\x92\x65\x8a\xde\xd9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fetch",
        .unit = "FET",
        .contract_address =
            "\x1d\x28\x7c\xc2\x5d\xad\x7c\xca\xf7\x6a\x26\xbc\x66\x0c\x5f\x7c\x8e\x2a\x05\xbd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fidelium Token",
        .unit = "FID",
        .contract_address =
            "\x52\xfb\x36\xc8\x3a\xd3\x3c\x18\x24\x91\x2f\xc8\x10\x71\xca\x5e\xeb\x8a\xb3\x90",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Knoxstertoken",
        .unit = "FKX",
        .contract_address =
            "\x00\x9e\x86\x49\x23\xb4\x92\x63\xc7\xf1\x0d\x19\xb7\xf8\xab\x7a\x9a\x5a\xad\x33",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Flixx",
        .unit = "FLIXX",
        .contract_address =
            "\xf0\x4a\x8a\xc5\x53\xfc\xed\xb5\xba\x99\xa6\x47\x99\x15\x58\x26\xc1\x36\xb0\xbe",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fire Lotto",
        .unit = "FLOT",
        .contract_address =
            "\x04\x93\x99\xa6\xb0\x48\xd5\x29\x71\xf7\xd1\x22\xae\x21\xa1\x53\x27\x22\x28\x5f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FLIP",
        .unit = "FLP",
        .contract_address =
            "\x3a\x1b\xda\x28\xad\xb5\xb0\xa8\x12\xa7\xcf\x10\xa1\x95\x0c\x92\x0f\x79\xbc\xd3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FluzFluz",
        .unit = "FLUZ",
        .contract_address =
            "\x95\x4b\x5d\xe0\x9a\x55\xe5\x97\x55\xac\xbd\xa2\x9e\x1e\xb7\x4a\x45\xd3\x01\x75",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FundRequest",
        .unit = "FND",
        .contract_address =
            "\x4d\xf4\x7b\x49\x69\xb2\x91\x1c\x96\x65\x06\xe3\x59\x2c\x41\x38\x94\x93\x95\x3b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FNKOSToken",
        .unit = "FNKOS",
        .contract_address =
            "\x07\x07\x68\x1f\x34\x4d\xeb\x24\x18\x40\x37\xfc\x02\x28\x85\x6f\x21\x37\xb0\x2e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FinTab",
        .unit = "FNTB",
        .contract_address =
            "\xbd\x4b\x60\xa1\x38\xb3\xfc\xe3\x58\x4e\xa0\x1f\x50\xc0\x90\x8c\x18\xf9\x67\x7a",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FOAM Token",
        .unit = "FOAM",
        .contract_address =
            "\x49\x46\xfc\xea\x7c\x69\x26\x06\xe8\x90\x80\x02\xe5\x5a\x58\x2a\xf4\x4a\xc1\x21",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FoodCoin",
        .unit = "FOOD",
        .contract_address =
            "\x2a\x09\x3B\xcF\x0C\x98\xEf\x74\x4B\xb6\xF6\x9D\x74\xf2\xF8\x56\x05\x32\x42\x90",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FOTA",
        .unit = "FOTA",
        .contract_address =
            "\x42\x70\xbb\x23\x8f\x6d\xd8\xb1\xc3\xca\x01\xf9\x6c\xa6\x5b\x26\x47\xc0\x6d\x3c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Freyr Coin",
        .unit = "FREC",
        .contract_address =
            "\x17\xe6\x7d\x1c\xb4\xe3\x49\xb9\xca\x4b\xc3\xe1\x7c\x7d\xf2\xa3\x97\xa7\xbb\x64",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Free Coin",
        .unit = "FREE",
        .contract_address =
            "\x2f\x14\x1c\xe3\x66\xa2\x46\x2f\x02\xce\xa3\xd1\x2c\xf9\x3e\x4d\xca\x49\xe4\xfd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fitrova",
        .unit = "FRV",
        .contract_address =
            "\x48\xdf\x4e\x02\x96\xf9\x08\xce\xab\x04\x28\xa5\x18\x2d\x19\xb3\x1f\xc0\x37\xd6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fusion",
        .unit = "FSN",
        .contract_address =
            "\xd0\x35\x2a\x01\x9e\x9a\xb9\xd7\x57\x77\x6f\x53\x23\x77\xaa\xeb\xd3\x6f\xd5\x41",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fabric Token",
        .unit = "FT",
        .contract_address =
            "\x78\xa7\x3B\x6C\xBc\x5D\x18\x3C\xE5\x6e\x78\x6f\x6e\x90\x5C\xaD\xEC\x63\x54\x7B",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FTI",
        .unit = "FTI",
        .contract_address =
            "\x94\x3e\xd8\x52\xda\xdb\x5c\x39\x38\xec\xdc\x68\x83\x71\x8d\xf8\x14\x2d\xe4\xc8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fantom Token",
        .unit = "FTM",
        .contract_address =
            "\x4e\x15\x36\x1f\xd6\xb4\xbb\x60\x9f\xa6\x3c\x81\xa2\xbe\x19\xd8\x73\x71\x78\x70",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fountain 3",
        .unit = "FTN",
        .contract_address =
            "\x56\x32\x5d\x18\x0e\xc3\x87\x8a\x90\x28\xaf\xc7\xb0\xed\xce\xe7\x48\x6c\xc9\xdf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FarmaTrust Token",
        .unit = "FTT",
        .contract_address =
            "\x2a\xec\x18\xc5\x50\x0f\x21\x35\x9c\xe1\xbe\xa5\xdc\x17\x77\x34\x4d\xf4\xc0\xdc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FintruX Network",
        .unit = "FTX",
        .contract_address =
            "\xd5\x59\xf2\x02\x96\xff\x48\x95\xda\x39\xb5\xbd\x9a\xdd\x54\xb4\x42\x59\x6a\x61",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FUTURAX",
        .unit = "FTXT",
        .contract_address =
            "\x41\x87\x5c\x23\x32\xb0\x87\x7c\xdf\xaa\x69\x9b\x64\x14\x02\xb7\xd4\x64\x2c\x32",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Finally Usable Crypto Karma",
        .unit = "FUCK",
        .contract_address =
            "\x65\xbe\x44\xc7\x47\x98\x8f\xbf\x60\x62\x07\x69\x8c\x94\x4d\xf4\x44\x2e\xfe\x19",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fuel Token",
        .unit = "FUEL",
        .contract_address =
            "\xea\x38\xea\xa3\xc8\x6c\x8f\x9b\x75\x15\x33\xba\x2e\x56\x2d\xeb\x9a\xcd\xed\x40",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FunFair",
        .unit = "FUN",
        .contract_address =
            "\x41\x9d\x0d\x8b\xdd\x9a\xf5\xe6\x06\xae\x22\x32\xed\x28\x5a\xff\x19\x0e\x71\x1b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Function X",
        .unit = "FX",
        .contract_address =
            "\x8c\x15\xef\x5b\x4b\x21\x95\x1d\x50\xe5\x3e\x4f\xbd\xa8\x29\x8f\xfa\xd2\x50\x57",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FuzeX",
        .unit = "FXT",
        .contract_address =
            "\x18\x29\xaa\x04\x5e\x21\xe0\xd5\x95\x80\x02\x4a\x95\x1d\xb4\x80\x96\xe0\x17\x82",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Flyp.me",
        .unit = "FYP",
        .contract_address =
            "\x8f\x09\x21\xf3\x05\x55\x62\x41\x43\xd4\x27\xb3\x40\xb1\x15\x69\x14\x88\x2c\x10",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HASHGARD",
        .unit = "GARD",
        .contract_address =
            "\x5c\x64\x03\x1c\x62\x06\x18\x65\xe5\xfd\x0f\x53\xd3\xcd\xae\xf8\x0f\x72\xe9\x9d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GAT",
        .unit = "GAT",
        .contract_address =
            "\x68\x71\x74\xf8\xc4\x9c\xeb\x77\x29\xd9\x25\xc3\xa9\x61\x50\x7e\xa4\xac\x7b\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gems",
        .unit = "GEM",
        .contract_address =
            "\xc7\xbb\xa5\xb7\x65\x58\x1e\xfb\x2c\xdd\x26\x79\xdb\x5b\xea\x9e\xe7\x9b\x20\x1f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DAOstack",
        .unit = "GEN",
        .contract_address =
            "\x54\x3f\xf2\x27\xf6\x4a\xa1\x7e\xa1\x32\xbf\x98\x86\xca\xb5\xdb\x55\xdc\xad\xdf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GENE TOKEN (PARKGENE)",
        .unit = "GENE",
        .contract_address =
            "\x6d\xd4\xe4\xaa\xd2\x9a\x40\xed\xd6\xa4\x09\xb9\xc1\x62\x51\x86\xc9\x85\x5b\x4d",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Guaranteed Entrance Token",
        .unit = "GET",
        .contract_address =
            "\x8a\x85\x42\x88\xa5\x97\x60\x36\xa7\x25\x87\x91\x64\xca\x3e\x91\xd3\x0c\x6a\x1b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gimli",
        .unit = "GIM",
        .contract_address =
            "\xae\x4f\x56\xf0\x72\xc3\x4c\x0a\x65\xb3\xae\x3e\x4d\xb7\x97\xd8\x31\x43\x9d\x93",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gladius",
        .unit = "GLA",
        .contract_address =
            "\x71\xd0\x1d\xb8\xd6\xa2\xfb\xea\x7f\x8d\x43\x45\x99\xc2\x37\x98\x0c\x23\x4e\x4c",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GMB",
        .unit = "GMB",
        .contract_address =
            "\xa0\x00\x8f\x51\x0f\xe9\xee\x69\x6e\x7e\x32\x0c\x9e\x5c\xbf\x61\xe2\x77\x91\xee",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Global Messaging Token",
        .unit = "GMT",
        .contract_address =
            "\xb3\xbd\x49\xe2\x8f\x8f\x83\x2b\x8d\x1e\x24\x61\x06\x99\x1e\x54\x6c\x32\x35\x02",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gnosis",
        .unit = "GNO",
        .contract_address =
            "\x68\x10\xe7\x76\x88\x0c\x02\x93\x3d\x47\xdb\x1b\x9f\xc0\x59\x08\xe5\x38\x6b\x96",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Golem",
        .unit = "GNT",
        .contract_address =
            "\xa7\x44\x76\x44\x31\x19\xA9\x42\xdE\x49\x85\x90\xFe\x1f\x24\x54\xd7\xD4\xaC\x0d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Genaro X",
        .unit = "GNX",
        .contract_address =
            "\x6e\xc8\xa2\x4c\xab\xdc\x33\x9a\x06\xa1\x72\xf8\x22\x3e\xa5\x57\x05\x5a\xda\xa5",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GOToken",
        .unit = "GOT",
        .contract_address =
            "\x61\x3f\xa2\xa6\xe6\xda\xa7\x0c\x65\x90\x60\xe8\x6b\xa1\x44\x3d\x26\x79\xc9\xd7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GoToken",
        .unit = "GOT",
        .contract_address =
            "\x42\x3b\x5f\x62\xb3\x28\xd0\xd6\xd4\x48\x70\xf4\xee\xe3\x16\xbe\xfa\x0b\x2d\xf5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GRID",
        .unit = "GRID",
        .contract_address =
            "\x12\xb1\x9d\x3e\x2c\xcc\x14\xda\x04\xfa\xe3\x3e\x63\x65\x2c\xe4\x69\xb3\xf2\xfd",
        .decimals = 12,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "GreenMed",
        .unit = "GRMD",
        .contract_address =
            "\xb4\x44\x20\x8c\xb0\x51\x6c\x15\x01\x78\xfc\xf9\xa5\x26\x04\xbc\x04\xa1\xac\xea",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Global Social Chain",
        .unit = "GSC",
        .contract_address =
            "\x22\x8b\xa5\x14\x30\x9f\xfd\xf0\x3a\x81\xa2\x05\xa6\xd0\x40\xe4\x29\xd6\xe8\x0c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Game.com Token",
        .unit = "GTC",
        .contract_address =
            "\xB7\x08\x35\xD7\x82\x2e\xBB\x94\x26\xB5\x65\x43\xE3\x91\x84\x6C\x10\x7b\xd3\x2C",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gifto",
        .unit = "GTO",
        .contract_address =
            "\xc5\xbb\xae\x50\x78\x1b\xe1\x66\x93\x06\xb9\xe0\x01\xef\xf5\x7a\x29\x57\xb0\x9d",
        .decimals = 5,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gemini dollar",
        .unit = "GUSD",
        .contract_address =
            "\x05\x6f\xd4\x09\xe1\xd7\xa1\x24\xbd\x70\x17\x45\x9d\xfe\xa2\xf3\x87\xb6\xd5\xcd",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Genesis Vision",
        .unit = "GVT",
        .contract_address =
            "\x10\x3c\x3A\x20\x9d\xa5\x9d\x3E\x7C\x4A\x89\x30\x7e\x66\x52\x1e\x08\x1C\xFD\xF0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Gene Source Code Chain",
        .unit = "Gene",
        .contract_address =
            "\x88\x41\x81\x55\x4d\xfa\x9e\x57\x8d\x36\x37\x99\x19\xc0\x5c\x25\xdc\x4a\x15\xbb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Guppy",
        .unit = "Guppy",
        .contract_address =
            "\xf7\xb0\x98\x29\x8f\x7c\x69\xfc\x14\x61\x0b\xf7\x1d\x5e\x02\xc6\x07\x92\x89\x4c",
        .decimals = 3,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HAC Token",
        .unit = "HAC",
        .contract_address =
            "\x43\x56\x7e\xb7\x86\x38\xA5\x5b\xbE\x51\xE9\xf9\xFB\x5B\x2D\x7A\xD1\xF1\x25\xaa",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ShowHand",
        .unit = "HAND",
        .contract_address =
            "\x48\xc1\xb2\xf3\xef\xa8\x5f\xba\xfb\x2a\xb9\x51\xbf\x4b\xa8\x60\xa0\x8c\xdb\xb7",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hawala",
        .unit = "HAT",
        .contract_address =
            "\x90\x02\xd4\x48\x5b\x75\x94\xe3\xe8\x50\xf0\xa2\x06\x71\x3b\x30\x51\x13\xf6\x9e",
        .decimals = 12,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HeartBout",
        .unit = "HB",
        .contract_address =
            "\x87\x7c\x7d\xeb\x5e\xb1\xfc\x5f\xaa\xd3\x0c\x71\xe3\xa6\xe3\x9d\xc8\xb1\x51\x9f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hubiits",
        .unit = "HBT",
        .contract_address =
            "\xdd\x6c\x68\xbb\x32\x46\x2e\x01\x70\x50\x11\xa4\xe2\xad\x1a\x60\x74\x0f\x21\x7f",
        .decimals = 15,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HashBX",
        .unit = "HBX",
        .contract_address =
            "\x6f\xe3\x55\xc6\x2c\x6f\xaf\x69\x46\xce\x88\x8f\xfa\xba\x9f\xd1\x23\x55\xae\x27",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Helbiz",
        .unit = "HBZ",
        .contract_address =
            "\xe3\x4e\x19\x44\xe7\x76\xf3\x9b\x92\x52\x79\x0a\x05\x27\xeb\xda\x64\x7a\xe6\x68",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hedge",
        .unit = "HDG",
        .contract_address =
            "\xff\xe8\x19\x6b\xc2\x59\xe8\xde\xdc\x54\x4d\x93\x57\x86\xaa\x47\x09\xec\x3e\x64",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HEDG",
        .unit = "HEDG",
        .contract_address =
            "\xf1\x29\x04\x73\xe2\x10\xb2\x10\x8a\x85\x23\x7f\xbc\xd7\xb6\xeb\x42\xcc\x65\x4f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HELP",
        .unit = "HELP",
        .contract_address =
            "\xbb\xc2\x04\x5d\x33\x5c\xb2\x24\x22\x8f\x18\x50\xb2\x91\x73\xd9\xd7\xd7\xb9\x89",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HeroNodeToken",
        .unit = "HER",
        .contract_address =
            "\x49\x1c\x9a\x23\xdb\x85\x62\x3e\xed\x45\x5a\x8e\xfd\xd6\xab\xa9\xb9\x11\xc5\xdf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hero Origen",
        .unit = "HERO",
        .contract_address =
            "\x02\x58\x5e\x4a\x14\xda\x27\x4d\x02\xdf\x09\xb2\x22\xd4\x60\x6b\x10\xa4\xe9\x40",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HireGo",
        .unit = "HGO",
        .contract_address =
            "\xaa\xa8\x91\x05\xda\xb8\x22\xdb\xc9\xa6\xde\x64\xa2\x3d\x04\x5d\x99\xd5\xfd\x36",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HelloGold",
        .unit = "HGT",
        .contract_address =
            "\xba\x21\x84\x52\x0a\x1c\xc4\x9a\x61\x59\xc5\x7e\x61\xe1\x84\x4e\x08\x56\x15\xb6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hacken",
        .unit = "HKN",
        .contract_address =
            "\x9e\x6b\x2b\x11\x54\x2f\x2b\xc5\x2f\x30\x29\x07\x7a\xce\x37\xe8\xfd\x83\x8d\x7f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hms Token",
        .unit = "HMC",
        .contract_address =
            "\xAa\x0b\xb1\x0C\xEc\x1f\xa3\x72\xeb\x3A\xbc\x17\xC9\x33\xFC\x6b\xa8\x63\xDD\x9E",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Humaniq",
        .unit = "HMQ",
        .contract_address =
            "\xcb\xcc\x0f\x03\x6e\xd4\x78\x8f\x63\xfc\x0f\xee\x32\x87\x3d\x6a\x74\x87\xb9\x08",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HOLD",
        .unit = "HOLD",
        .contract_address =
            "\xd6\xe1\x40\x1a\x07\x99\x22\x46\x9e\x9b\x96\x5c\xb0\x90\xea\x6f\xf6\x4c\x68\x39",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Horse",
        .unit = "HORSE",
        .contract_address =
            "\x5b\x07\x51\x71\x3b\x25\x27\xd7\xf0\x02\xc0\xc4\xe2\xa3\x7e\x12\x19\x61\x0a\x6b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HoloToken",
        .unit = "HOT",
        .contract_address =
            "\x6c\x6e\xe5\xe3\x1d\x82\x8d\xe2\x41\x28\x2b\x96\x06\xc8\xe9\x8e\xa4\x85\x26\xe2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hydro Protocol",
        .unit = "HOT",
        .contract_address =
            "\x9a\xf8\x39\x68\x7f\x6c\x94\x54\x2a\xc5\xec\xe2\xe3\x17\xda\xae\x35\x54\x93\xa1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HPBCoin",
        .unit = "HPB",
        .contract_address =
            "\x38\xc6\xa6\x83\x04\xcd\xef\xb9\xbe\xc4\x8b\xbf\xaa\xba\x5c\x5b\x47\x81\x8b\xb2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HuobiPoolToken",
        .unit = "HPT",
        .contract_address =
            "\xa6\x6d\xaa\x57\x43\x20\x24\x02\x3d\xb6\x54\x77\xba\x87\xd4\xe7\xf5\xf9\x52\x13",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HyperQuant Token",
        .unit = "HQT",
        .contract_address =
            "\x3e\x1d\x5a\x85\x5a\xd9\xd9\x48\x37\x3a\xe6\x8e\x4f\xe1\xf0\x94\x61\x2b\x13\x22",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HashCoin",
        .unit = "HSC",
        .contract_address =
            "\x2b\xba\x3c\xf6\xde\x60\x58\xcc\x1b\x44\x57\xce\x00\xde\xb3\x59\xe2\x70\x3d\x7f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Helper Search Token",
        .unit = "HSN",
        .contract_address =
            "\x56\x73\x00\xe1\x4f\x8d\x67\xe1\xf6\x72\x0a\x95\x29\x1d\xce\x25\x11\xa8\x67\x23",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Decision Token",
        .unit = "HST",
        .contract_address =
            "\x55\x4c\x20\xb7\xc4\x86\xbe\xee\x43\x92\x77\xb4\x54\x0a\x43\x45\x66\xdc\x4c\x02",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HuobiToken",
        .unit = "HT",
        .contract_address =
            "\x6f\x25\x96\x37\xdc\xd7\x4c\x76\x77\x81\xe3\x7b\xc6\x13\x3c\xd6\xa6\x8a\xa1\x61",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hotbit Token",
        .unit = "HTB",
        .contract_address =
            "\x6b\xe6\x18\x33\xfc\x43\x81\x99\x0e\x82\xd7\xd4\xa9\xf4\xc9\xb3\xf6\x7e\xa9\x41",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hurify Token",
        .unit = "HUR",
        .contract_address =
            "\xcd\xb7\xec\xfd\x34\x03\xee\xf3\x88\x2c\x65\xb7\x61\xef\x9b\x50\x54\x89\x0a\x47",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hiveterminal Token",
        .unit = "HVN",
        .contract_address =
            "\xC0\xEb\x85\x28\x5d\x83\x21\x7C\xD7\xc8\x91\x70\x2b\xcb\xC0\xFC\x40\x1E\x2D\x9D",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Hydro",
        .unit = "HYDRO",
        .contract_address =
            "\xeb\xbd\xf3\x02\xc9\x40\xc6\xbf\xd4\x9c\x6b\x16\x5f\x45\x7f\xdb\x32\x46\x49\xbc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IcoBay",
        .unit = "IBT",
        .contract_address =
            "\x79\x14\x25\x15\x69\x56\xe3\x9f\x2a\xb8\xab\x06\xb7\x9d\xe1\x89\xc1\x8e\x95\xe5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IBTC",
        .unit = "IBTC",
        .contract_address =
            "\x07\x84\xdb\xab\xb6\xc6\x83\x4b\xdd\xfb\x7c\xfe\xe1\x16\xba\x04\x9e\x5d\xaf\xab",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IDICE",
        .unit = "ICE",
        .contract_address =
            "\x5a\x84\x96\x9b\xb6\x63\xfb\x64\xf6\xd0\x15\xdc\xf9\xf6\x22\xae\xdc\x79\x67\x50",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ICONOMI",
        .unit = "ICN",
        .contract_address =
            "\x88\x86\x66\xCA\x69\xE0\xf1\x78\xDE\xD6\xD7\x5b\x57\x26\xCe\xe9\x9A\x87\xD6\x98",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Iconiq Lab Token",
        .unit = "ICNQ",
        .contract_address =
            "\xb3\xe2\xcb\x7c\xcc\xfe\x13\x9f\x8f\xf8\x40\x13\x82\x3b\xf2\x2d\xa6\xb6\x39\x0a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ICOS",
        .unit = "ICOS",
        .contract_address =
            "\x01\x4B\x50\x46\x65\x90\x34\x0D\x41\x30\x7C\xc5\x4D\xCe\xe9\x90\xc8\xD5\x8a\xa8",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ICOCalendar.Today",
        .unit = "ICT",
        .contract_address =
            "\x18\x7c\x4b\x0e\x38\x19\x01\x7a\x5c\xf0\x7a\xf8\x1a\x4e\x2b\x16\x16\x6a\xab\xc6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ICON",
        .unit = "ICX",
        .contract_address =
            "\xb5\xa5\xf2\x26\x94\x35\x2c\x15\xb0\x03\x23\x84\x4a\xd5\x45\xab\xb2\xb1\x10\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "indaHash Coin",
        .unit = "IDH",
        .contract_address =
            "\x51\x36\xc9\x8a\x80\x81\x1c\x3f\x46\xbd\xda\x8b\x5c\x45\x55\xcf\xd9\xf8\x12\xf0",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rupiah Token",
        .unit = "IDRT",
        .contract_address =
            "\x99\x8F\xFE\x1E\x43\xfA\xcf\xfb\x94\x1d\xc3\x37\xdD\x04\x68\xd5\x2b\xA5\xb4\x8A",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IDEX Membership",
        .unit = "IDXM",
        .contract_address =
            "\xcc\x13\xfc\x62\x7e\xff\xd6\xe3\x5d\x2d\x27\x06\xea\x3c\x4d\x73\x96\xc6\x10\xea",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ifoods Token",
        .unit = "IFOOD",
        .contract_address =
            "\x81\xe7\x4a\x3e\xa4\xba\xb2\x27\x7a\xa3\xb9\x41\xe9\xd9\xf3\x7b\x08\xac\x53\x74",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Feed",
        .unit = "IFT",
        .contract_address =
            "\x76\x54\x91\x5a\x1b\x82\xd6\xd2\xd0\xaf\xc3\x7c\x52\xaf\x55\x6e\xa8\x98\x3c\x7e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IG",
        .unit = "IG",
        .contract_address =
            "\x8a\x88\xf0\x4e\x0c\x90\x50\x54\xd2\xf3\x3b\x26\xbb\x3a\x46\xd7\x09\x1a\x03\x9a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Invictus Hyperion",
        .unit = "IHF",
        .contract_address =
            "\xaf\x12\x50\xfa\x68\xd7\xde\xcd\x34\xfd\x75\xde\x87\x42\xbc\x03\xb2\x9b\xd5\x8e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "I HOUSE TOKEN",
        .unit = "IHT",
        .contract_address =
            "\xed\xa8\xb0\x16\xef\xa8\xb1\x16\x12\x08\xcf\x04\x1c\xd8\x69\x72\xee\xe0\xf3\x1e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ether Kingdoms Token",
        .unit = "IMP",
        .contract_address =
            "\x48\xff\x53\x77\x7f\x74\x7c\xfb\x69\x41\x01\x22\x2a\x94\x4d\xe0\x70\xc1\x5d\x36",
        .decimals = 7,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MoneyToken",
        .unit = "IMT",
        .contract_address =
            "\x13\x11\x9e\x34\xe1\x40\x09\x7a\x50\x7b\x07\xa5\x56\x4b\xde\x1b\xc3\x75\xd9\xe6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Insight Chain",
        .unit = "INB",
        .contract_address =
            "\x17\xaa\x18\xa4\xb6\x4a\x55\xab\xed\x7f\xa5\x43\xf2\xba\x4e\x91\xf2\xdc\xe4\x82",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Incodium",
        .unit = "INCO",
        .contract_address =
            "\x4d\x8b\xfe\x7e\xa0\xf4\x64\x86\xfd\x40\xfc\x4d\xf6\x0c\xf3\x9f\x75\x68\xbe\xe8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "INCX Coin",
        .unit = "INCX",
        .contract_address =
            "\xa9\x84\xa9\x27\x31\xc0\x88\xf1\xea\x4d\x53\xb7\x1a\x25\x65\xa3\x99\xf7\xd8\xd5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Indorse",
        .unit = "IND",
        .contract_address =
            "\xf8\xe3\x86\xed\xa8\x57\x48\x4f\x5a\x12\xe4\xb5\xda\xa9\x98\x4e\x06\xe7\x37\x05",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IUNGO",
        .unit = "ING",
        .contract_address =
            "\x24\xdd\xff\x6d\x8b\x8a\x42\xd8\x35\xaf\x3b\x44\x0d\xe9\x1f\x33\x86\x55\x4a\xa4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ino Coin",
        .unit = "INO",
        .contract_address =
            "\xc9\x85\x9f\xcc\xc8\x76\xe6\xb4\xb3\xc7\x49\xc5\xd2\x9e\xa0\x4f\x48\xac\xb7\x4f",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Insolar",
        .unit = "INS",
        .contract_address =
            "\x5b\x2e\x4a\x70\x0d\xfb\xc5\x60\x06\x1e\x95\x7e\xde\xc8\xf6\xee\xeb\x74\xa3\x20",
        .decimals = 10,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Insights Network",
        .unit = "INSTAR",
        .contract_address =
            "\xc7\x2f\xe8\xe3\xdd\x5b\xef\x0f\x9f\x31\xf2\x59\x39\x9f\x30\x12\x72\xef\x2a\x2d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Internet Node Token",
        .unit = "INT",
        .contract_address =
            "\x0b\x76\x54\x4f\x6c\x41\x3a\x55\x5f\x30\x9b\xf7\x62\x60\xd1\xe0\x23\x77\xc0\x2a",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Invacio Coin",
        .unit = "INV",
        .contract_address =
            "\xec\xe8\x36\x17\xdb\x20\x8a\xd2\x55\xad\x4f\x45\xda\xf8\x1e\x25\x13\x75\x35\xbb",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "InterValue",
        .unit = "INVE",
        .contract_address =
            "\xda\xc4\xae\x18\x8a\xce\x3c\x89\x85\x76\x5e\xdc\x6c\x9b\x47\x39\xd4\x84\x5d\xdc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Internxt",
        .unit = "INXT",
        .contract_address =
            "\xa8\x00\x6c\x4c\xa5\x6f\x24\xd6\x83\x67\x27\xd1\x06\x34\x93\x20\xdb\x7f\xef\x82",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IONChain Token",
        .unit = "IONC",
        .contract_address =
            "\xbc\x64\x7a\xad\x10\x11\x4b\x89\x56\x4c\x0a\x7a\xab\xe5\x42\xbd\x0c\xf2\xc5\xaf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IOSToken",
        .unit = "IOST",
        .contract_address =
            "\xfa\x1a\x85\x6c\xfa\x34\x09\xcf\xa1\x45\xfa\x4e\x20\xeb\x27\x0d\xf3\xeb\x21\xab",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IoTeX Network",
        .unit = "IOTX",
        .contract_address =
            "\x6f\xb3\xe0\xa2\x17\x40\x7e\xff\xf7\xca\x06\x2d\x46\xc2\x6e\x5d\x60\xa1\x4d\x69",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IPC",
        .unit = "IPC",
        .contract_address =
            "\x62\x2c\xd5\x4d\xeb\x2b\xb7\xa0\x51\x51\x51\x92\x41\x71\x09\xbc\xf3\xfe\x09\x8f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "InsurePal",
        .unit = "IPL",
        .contract_address =
            "\x64\xcd\xf8\x19\xd3\xe7\x5a\xc8\xec\x21\x7b\x34\x96\xd7\xce\x16\x7b\xe4\x2e\x80",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IPSX",
        .unit = "IPSX",
        .contract_address =
            "\x00\x1f\x0a\xa5\xda\x15\x58\x5e\x5b\x23\x05\xdb\xab\x2b\xac\x42\x5e\xa7\x10\x07",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "iQeon",
        .unit = "IQN",
        .contract_address =
            "\x0d\xb8\xd8\xb7\x6b\xc3\x61\xba\xcb\xb7\x2e\x2c\x49\x1e\x06\x08\x5a\x97\xab\x31",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Insureum Token",
        .unit = "ISR",
        .contract_address =
            "\xd4\xa2\x93\xae\x8b\xb9\xe0\xbe\x12\xe9\x9e\xb1\x9d\x48\x23\x9e\x8c\x83\xa1\x36",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IOT Chain",
        .unit = "ITC",
        .contract_address =
            "\x5e\x6b\x6d\x9a\xba\xd9\x09\x3f\xdc\x86\x1e\xa1\x60\x0e\xba\x1b\x35\x5c\xd9\x40",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ITL (Italian Lira)",
        .unit = "ITL",
        .contract_address =
            "\x12\x2A\x86\xb5\xDF\xF2\xD0\x85\xAf\xB4\x96\x00\xb4\xcd\x73\x75\xD0\xd9\x4A\x5f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Intelligent Trading Foundation",
        .unit = "ITT",
        .contract_address =
            "\x0a\xef\x06\xdc\xcc\xc5\x31\xe5\x81\xf0\x44\x00\x59\xe6\xff\xcc\x20\x60\x39\xee",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IvyKoin Public Network Tokens",
        .unit = "IVY",
        .contract_address =
            "\xA4\xeA\x68\x7A\x2A\x7F\x29\xcF\x2d\xc6\x6B\x39\xc6\x8e\x44\x11\xC0\xD0\x0C\x49",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "IXE Token",
        .unit = "IXE",
        .contract_address =
            "\x7a\x07\xe1\xa0\xc2\x51\x4d\x51\x13\x21\x83\xec\xfe\xa2\xa8\x80\xec\x3b\x76\x48",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "iXledger",
        .unit = "IXT",
        .contract_address =
            "\xfc\xa4\x79\x62\xd4\x5a\xdf\xdf\xd1\xab\x2d\x97\x23\x15\xdb\x4c\xe7\xcc\xf0\x94",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "J8T",
        .unit = "J8T",
        .contract_address =
            "\x0d\x26\x2e\x5d\xc4\xa0\x6a\x0f\x1c\x90\xce\x79\xc7\xa6\x0c\x09\xdf\xc8\x84\xe4",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Jesus Coin",
        .unit = "JC",
        .contract_address =
            "\xe2\xd8\x2d\xc7\xda\x0e\x6f\x88\x2e\x96\x84\x64\x51\xf4\xfa\xbc\xc8\xf9\x05\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Jetcoin",
        .unit = "JET",
        .contract_address =
            "\x87\x27\xc1\x12\xc7\x12\xc4\xa0\x33\x71\xac\x87\xa7\x4d\xd6\xab\x10\x4a\xf7\x68",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Jibrel Network Token",
        .unit = "JNT",
        .contract_address =
            "\xa5\xfd\x1a\x79\x1c\x4d\xfc\xaa\xcc\x96\x3d\x4f\x73\xc6\xae\x58\x24\x14\x9e\xa7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Jobchain",
        .unit = "JOB",
        .contract_address =
            "\x17\x28\x0d\xa0\x53\x59\x6e\x09\x76\x04\x83\x9c\x61\xa2\xef\x5e\xfb\x7d\x49\x3f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Jury.Online",
        .unit = "JOT",
        .contract_address =
            "\xdb\x45\x5c\x71\xc1\xbc\x2d\xe4\xe8\x0c\xa4\x51\x18\x40\x41\xef\x32\x05\x40\x01",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "JavaScript",
        .unit = "JS",
        .contract_address =
            "\x50\x46\xe8\x60\xff\x27\x4f\xb8\xc6\x61\x06\xb0\xff\xb8\x15\x58\x49\xfb\x07\x87",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "JSE Token",
        .unit = "JSE",
        .contract_address =
            "\x2d\x18\x40\x14\xb5\x65\x8c\x45\x34\x43\xaa\x87\xc8\xe9\xc4\xd5\x72\x85\x62\x0b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "KAN",
        .unit = "KAN",
        .contract_address =
            "\x14\x10\x43\x4b\x03\x46\xf5\xbe\x67\x8d\x0f\xb5\x54\xe5\xc7\xab\x62\x0f\x8f\x4a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Katalyse",
        .unit = "KAT",
        .contract_address =
            "\x88\xFC\xFB\xc2\x2C\x6d\x3d\xBa\xa2\x5a\xF4\x78\xC5\x78\x97\x83\x39\xBD\xe7\x7a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "KaratBank Coin",
        .unit = "KBC",
        .contract_address =
            "\xf3\x58\x66\x84\x10\x7c\xe0\x85\x9c\x44\xaa\x2b\x2e\x0f\xb8\xcd\x87\x31\xa1\x5a",
        .decimals = 7,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kcash",
        .unit = "KCASH",
        .contract_address =
            "\x32\xd7\x48\x96\xf0\x52\x04\xd1\xb6\xae\x7b\x0a\x3c\xeb\xd7\xfc\x0c\xd8\xf9\xc7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kucoin Shares",
        .unit = "KCS",
        .contract_address =
            "\x03\x9b\x56\x49\xa5\x99\x67\xe3\xe9\x36\xd7\x47\x1f\x9c\x37\x00\x10\x0e\xe1\xab",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SelfKey",
        .unit = "KEY",
        .contract_address =
            "\x4c\xc1\x93\x56\xf2\xd3\x73\x38\xb9\x80\x2a\xa8\xe8\xfc\x58\xb0\x37\x32\x96\xe7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "KickToken",
        .unit = "KICK",
        .contract_address =
            "\xc1\x2d\x1c\x73\xee\x7d\xc3\x61\x5b\xa4\xe3\x7e\x4a\xbf\xdb\xdd\xfa\x38\x90\x7e",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kin",
        .unit = "KIN",
        .contract_address =
            "\x81\x8f\xc6\xc2\xec\x59\x86\xbc\x6e\x2c\xbf\x00\x93\x9d\x90\x55\x6a\xb1\x2c\xe5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kind Ads Token",
        .unit = "KIND",
        .contract_address =
            "\x46\x18\x51\x9d\xe4\xc3\x04\xf3\x44\x4f\xfa\x7f\x81\x2d\xdd\xc2\x97\x1c\xc6\x88",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "KyberNetwork",
        .unit = "KNC",
        .contract_address =
            "\xdd\x97\x4d\x5c\x2e\x29\x28\xde\xa5\xf7\x1b\x98\x25\xb8\xb6\x46\x68\x6b\xd2\x00",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "KanadeCoin",
        .unit = "KNDC",
        .contract_address =
            "\x8e\x56\x10\xab\x5e\x39\xd2\x68\x28\x16\x76\x40\xea\x29\x82\x3f\xe1\xdd\x58\x43",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kora Network Token",
        .unit = "KNT",
        .contract_address =
            "\xff\x5c\x25\xd2\xf4\x0b\x47\xc4\xa3\x7f\x98\x9d\xe9\x33\xe2\x65\x62\xef\x0a\xc0",
        .decimals = 16,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kryll",
        .unit = "KRL",
        .contract_address =
            "\x46\x4e\xbe\x77\xc2\x93\xe4\x73\xb4\x8c\xfe\x96\xdd\xcf\x88\xfc\xf7\xbf\xda\xc0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Kuende Token",
        .unit = "KUE",
        .contract_address =
            "\xdf\x13\x38\xfb\xaf\xe7\xaf\x17\x89\x15\x16\x27\xb8\x86\x78\x1b\xa5\x56\xef\x9a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "4NEW",
        .unit = "KWATT",
        .contract_address =
            "\x24\x1b\xa6\x72\x57\x4a\x78\xa3\xa6\x04\xcd\xd0\xa9\x44\x29\xa7\x3a\x84\xa3\x24",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "KWHCoin",
        .unit = "KWH",
        .contract_address =
            "\xb8\xdd\xc9\x30\xc2\xba\xb6\xc7\x16\x10\xa2\xbe\x63\x90\x36\xe8\x29\xf9\xc1\x0b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Almeela",
        .unit = "KZE",
        .contract_address =
            "\x8d\xe6\x7d\x55\xc5\x85\x40\x80\x76\x01\xdb\xf1\x25\x95\x37\xbc\x2d\xff\xc8\x4d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LAtoken",
        .unit = "LA",
        .contract_address =
            "\xe5\x03\x65\xf5\xd6\x79\xcb\x98\xa1\xdd\x62\xd6\xf6\xe5\x8e\x59\x32\x1b\xcd\xdf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LALA",
        .unit = "LALA",
        .contract_address =
            "\xfd\x10\x7b\x47\x3a\xb9\x0e\x8f\xbd\x89\x87\x21\x44\xa3\xdc\x92\xc4\x0f\xa8\xc9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Lambda",
        .unit = "LAMB",
        .contract_address =
            "\x89\x71\xf9\xfd\x71\x96\xe5\xce\xe2\xc1\x03\x2b\x50\xf6\x56\x85\x5a\xf7\xdd\x26",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LatiumX",
        .unit = "LATX",
        .contract_address =
            "\x2f\x85\xe5\x02\xa9\x88\xaf\x76\xf7\xee\x6d\x83\xb7\xdb\x8d\x6c\x0a\x82\x3b\xf9",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LibraToken",
        .unit = "LBA",
        .contract_address =
            "\xfe\x5f\x14\x1b\xf9\x4f\xe8\x4b\xc2\x8d\xed\x0a\xb9\x66\xc1\x6b\x17\x49\x06\x57",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LocalCoinSwap Cryptoshare",
        .unit = "LCS",
        .contract_address =
            "\xaa\x19\x96\x1b\x6b\x85\x8d\x9f\x18\xa1\x15\xf2\x5a\xa1\xd9\x8a\xbc\x1f\xdb\xa8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LendConnect",
        .unit = "LCT",
        .contract_address =
            "\x05\xc7\x06\x5d\x64\x40\x96\xa4\xe4\xc3\xfe\x24\xaf\x86\xe3\x6d\xe0\x21\x07\x4b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Education",
        .unit = "LEDU",
        .contract_address =
            "\x5b\x26\xc5\xd0\x77\x2e\x5b\xba\xc8\xb3\x18\x2a\xe9\xa1\x3f\x9b\xb2\xd0\x37\x65",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "EthLend",
        .unit = "LEND",
        .contract_address =
            "\x80\xfB\x78\x4B\x7e\xD6\x67\x30\xe8\xb1\xDB\xd9\x82\x0a\xFD\x29\x93\x1a\xab\x03",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Bitfinex LEO Token",
        .unit = "LEO",
        .contract_address =
            "\x2a\xf5\xd2\xad\x76\x74\x11\x91\xd1\x5d\xfe\x7b\xf6\xac\x92\xd4\xbd\x91\x2c\xa3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Leverj",
        .unit = "LEV",
        .contract_address =
            "\x0f\x4c\xa9\x26\x60\xef\xad\x97\xa9\xa7\x0c\xb0\xfe\x96\x9c\x75\x54\x39\x77\x2c",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LinfinityCoin",
        .unit = "LFC",
        .contract_address =
            "\x95\xc9\xbd\x1f\x81\xce\xe7\x39\x1d\xa3\xea\xc8\x16\x93\xe6\x0f\x32\x92\xc1\xe0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Legends",
        .unit = "LGD",
        .contract_address =
            "\x59\x06\x1b\x6f\x26\xBB\x4A\x9c\xE5\x82\x8A\x19\xd3\x5C\xFD\x5A\x4B\x80\xF0\x56",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LGO Token",
        .unit = "LGO",
        .contract_address =
            "\x0a\x50\xc9\x3c\x76\x2f\xdd\x6e\x56\xd8\x62\x15\xc2\x4a\xaa\xd4\x3a\xb6\x29\xaa",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Lif",
        .unit = "LIF",
        .contract_address =
            "\xeb\x99\x51\x02\x16\x98\xb4\x2e\x43\x99\xf9\xcb\xb6\x26\x7a\xa3\x5f\x82\xd5\x9d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PureLifeCoin",
        .unit = "LIFE",
        .contract_address =
            "\xff\x18\xdb\xc4\x87\xb4\xc2\xe3\x22\x2d\x11\x59\x52\xba\xbf\xda\x8b\xa5\x2f\x5f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LikeCoin",
        .unit = "LIKE",
        .contract_address =
            "\x02\xf6\x1f\xd2\x66\xda\x6e\x8b\x10\x2d\x41\x21\xf5\xce\x7b\x99\x26\x40\xcf\x98",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LINA",
        .unit = "LINA",
        .contract_address =
            "\xc0\x5d\x14\x44\x2a\x51\x0d\xe4\xd3\xd7\x1a\x3d\x31\x65\x85\xaa\x0c\xe3\x2b\x50",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ChainLink Token",
        .unit = "LINK",
        .contract_address =
            "\x51\x49\x10\x77\x1a\xf9\xca\x65\x6a\xf8\x40\xdf\xf8\x3e\x82\x64\xec\xf9\x86\xca",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LionCoin",
        .unit = "LION",
        .contract_address =
            "\x21\x67\xfb\x82\x30\x9c\xf7\x65\x13\xe8\x3b\x25\x12\x3f\x8b\x05\x59\xd6\xb4\x8f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Linkey Token",
        .unit = "LKY",
        .contract_address =
            "\x49\xbd\x2d\xa7\x5b\x1f\x7a\xf1\xe4\xdf\xd6\xb1\x12\x5f\xec\xde\x59\xdb\xec\x58",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Linker Coin",
        .unit = "LNC",
        .contract_address =
            "\x6B\xEB\x41\x8F\xc6\xE1\x95\x82\x04\xaC\x8b\xad\xdC\xf1\x09\xB8\xE9\x69\x49\x66",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Lancer Token",
        .unit = "LNC",
        .contract_address =
            "\x63\xe6\x34\x33\x0a\x20\x15\x0d\xbb\x61\xb1\x56\x48\xbc\x73\x85\x5d\x6c\xcf\x07",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Lendingblock",
        .unit = "LND",
        .contract_address =
            "\x09\x47\xb0\xe6\xd8\x21\x37\x88\x05\xc9\x59\x82\x91\x38\x5c\xe7\xc7\x91\xa6\xb2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Link Platform",
        .unit = "LNK",
        .contract_address =
            "\xe2\xe6\xd4\xbe\x08\x6c\x69\x38\xb5\x3b\x22\x14\x48\x55\xee\xf6\x74\x28\x16\x39",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LNX Protocol",
        .unit = "LNX",
        .contract_address =
            "\x5e\x38\x45\xa1\xd7\x8d\xb5\x44\x61\x3e\xdb\xe4\x3d\xc1\xea\x49\x72\x66\xd3\xb8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LockTrip",
        .unit = "LOC",
        .contract_address =
            "\x5e\x33\x46\x44\x40\x10\x13\x53\x22\x26\x8a\x46\x30\xd2\xed\x5f\x8d\x09\x44\x6c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LOCIcoin",
        .unit = "LOCI",
        .contract_address =
            "\x9c\x23\xd6\x7a\xea\x7b\x95\xd8\x09\x42\xe3\x83\x6b\xcd\xf7\xe7\x08\xa7\x47\xc2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Loom",
        .unit = "LOOM",
        .contract_address =
            "\xa4\xe8\xc3\xec\x45\x61\x07\xea\x67\xd3\x07\x5b\xf9\xe3\xdf\x3a\x75\x82\x3d\xb0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LoopringCoin V2",
        .unit = "LRC",
        .contract_address =
            "\xbb\xbb\xca\x6a\x90\x1c\x92\x6f\x24\x0b\x89\xea\xcb\x64\x1d\x8a\xec\x7a\xea\xfd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Luna Stars",
        .unit = "LSTR",
        .contract_address =
            "\xc8\x0d\xae\x91\x29\x38\x9c\x4e\xf6\xe7\xdb\xfa\xee\x40\xf2\x8d\xe4\x9d\xe3\xa6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Level-Up Coin",
        .unit = "LUC",
        .contract_address =
            "\x5d\xbe\x29\x6f\x97\xb2\x3c\x4a\x6a\xa6\x18\x3d\x73\xe5\x74\xd0\x2b\xa5\xc7\x19",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Lunyr",
        .unit = "LUN",
        .contract_address =
            "\xfa\x05\xA7\x3F\xfE\x78\xef\x8f\x1a\x73\x94\x73\xe4\x62\xc5\x4b\xae\x65\x67\xD9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "LiteXToken",
        .unit = "LXT",
        .contract_address =
            "\xbc\x46\xd9\x96\x1a\x39\x32\xf7\xd6\xb6\x4a\xbf\xde\xc8\x0c\x18\x16\xc4\xb8\x35",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MATRIX AI Network",
        .unit = "MAN",
        .contract_address =
            "\xe2\x5b\xce\xc5\xd3\x80\x1c\xe3\xa7\x94\x07\x9b\xf9\x4a\xdf\x1b\x8c\xcd\x80\x2d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Decentraland",
        .unit = "MANA",
        .contract_address =
            "\x0f\x5d\x2f\xb2\x9f\xb7\xd3\xcf\xee\x44\x4a\x20\x02\x98\xf4\x68\x90\x8c\xc9\x42",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MidasProtocol",
        .unit = "MAS",
        .contract_address =
            "\x23\xcc\xc4\x33\x65\xd9\xdd\x38\x82\xea\xb8\x8f\x43\xd5\x15\x20\x8f\x83\x24\x30",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MasterNet",
        .unit = "MASH",
        .contract_address =
            "\xa0\xd4\x40\xc6\xda\x37\x89\x2d\xc0\x6e\xe7\x93\x0b\x2e\xed\xe0\x63\x4f\xd6\x81",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Matic Token",
        .unit = "MATIC",
        .contract_address =
            "\x7d\x1a\xfa\x7b\x71\x8f\xb8\x93\xdb\x30\xa3\xab\xc0\xcf\xc6\x08\xaa\xcf\xeb\xb0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MCAP",
        .unit = "MCAP",
        .contract_address =
            "\x93\xe6\x82\x10\x7d\x1e\x9d\xef\xb0\xb5\xee\x70\x1c\x71\x70\x7a\x4b\x2e\x46\xbc",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Musiconomi",
        .unit = "MCI",
        .contract_address =
            "\x13\x8a\x87\x52\x09\x3f\x4f\x9a\x79\xaa\xed\xf4\x8d\x4b\x92\x48\xfa\xb9\x3c\x9c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MCO",
        .unit = "MCO",
        .contract_address =
            "\xb6\x3b\x60\x6a\xc8\x10\xa5\x2c\xca\x15\xe4\x4b\xb6\x30\xfd\x42\xd8\xd1\xd8\x3d",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Moeda Loyalty Points",
        .unit = "MDA",
        .contract_address =
            "\x51\xdb\x5a\xd3\x5c\x67\x1a\x87\x20\x7d\x88\xfc\x11\xd5\x93\xac\x0c\x84\x15\xbd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MediShares",
        .unit = "MDS",
        .contract_address =
            "\x66\x18\x60\x08\xC1\x05\x06\x27\xF9\x79\xd4\x64\xeA\xBb\x25\x88\x60\x56\x3d\xbE",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Measurable Data Token",
        .unit = "MDT",
        .contract_address =
            "\x81\x4e\x09\x08\xb1\x2a\x99\xfe\xcf\x5b\xc1\x01\xbb\x5d\x0b\x8b\x5c\xdf\x7d\x26",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MEDX TOKEN",
        .unit = "MEDX",
        .contract_address =
            "\xfd\x1e\x80\x50\x8f\x24\x3e\x64\xce\x23\x4e\xa8\x8a\x5f\xd2\x82\x7c\x71\xd4\xb7",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BlockMesh",
        .unit = "MESH",
        .contract_address =
            "\xf0\x30\x45\xa4\xc8\x07\x7e\x38\xf3\xb8\xe2\xed\x33\xb8\xae\xe6\x9e\xdf\x86\x9f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Metronome",
        .unit = "MET",
        .contract_address =
            "\xa3\xd5\x8c\x4e\x56\xfe\xdc\xae\x3a\x7c\x43\xa7\x25\xae\xe9\xa7\x1f\x0e\xce\x4e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Metadium",
        .unit = "META",
        .contract_address =
            "\xde\x2f\x77\x66\xc8\xbf\x14\xca\x67\x19\x31\x28\x53\x5e\x5c\x74\x54\xf8\x38\x7c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MetaMorph",
        .unit = "METM",
        .contract_address =
            "\xfe\xf3\x88\x4b\x60\x3c\x33\xef\x8e\xd4\x18\x33\x46\xe0\x93\xa1\x73\xc9\x4d\xa6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SyncFab Smart Manufacturing Blockchain",
        .unit = "MFG",
        .contract_address =
            "\x67\x10\xc6\x34\x32\xa2\xde\x02\x95\x4f\xc0\xf8\x51\xdb\x07\x14\x6a\x6c\x03\x12",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mainframe Token",
        .unit = "MFT",
        .contract_address =
            "\xdf\x2c\x72\x38\x19\x8a\xd8\xb3\x89\x66\x65\x74\xf2\xd8\xbc\x41\x1a\x4b\x74\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MFTU",
        .unit = "MFTU",
        .contract_address =
            "\x05\xd4\x12\xce\x18\xf2\x40\x40\xbb\x3f\xa4\x5c\xf2\xc6\x9e\x50\x65\x86\xd8\xe8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MobileGo",
        .unit = "MGO",
        .contract_address =
            "\x40\x39\x50\x44\xAc\x3c\x0C\x57\x05\x19\x06\xdA\x93\x8B\x54\xBD\x65\x57\xF2\x12",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mobile Integrated Blockchain",
        .unit = "MIB",
        .contract_address =
            "\x14\x6d\x8d\x94\x20\x48\xad\x51\x74\x79\xc9\xba\xb1\x78\x87\x12\xaf\x18\x0f\xde",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mindexcoin",
        .unit = "MIC",
        .contract_address =
            "\x3a\x12\x37\xd3\x8d\x0f\xb9\x45\x13\xf8\x5d\x61\x67\x9c\xad\x7f\x38\x50\x72\x42",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MICROMINES",
        .unit = "MICRO",
        .contract_address =
            "\xbe\x6c\x8f\x28\x10\xef\x39\x42\x0d\x2d\xc2\x90\x1b\x84\x14\xc8\xc4\x5f\xee\x6d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Morpheus Infrastructure Token",
        .unit = "MITx",
        .contract_address =
            "\x4a\x52\x7d\x8f\xc1\x3c\x52\x03\xab\x24\xba\x09\x44\xf4\xcb\x14\x65\x8d\x1d\xb6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Maker",
        .unit = "MKR",
        .contract_address =
            "\x9f\x8f\x72\xaa\x93\x04\xc8\xb5\x93\xd5\x55\xf1\x2e\xf6\x58\x9c\xc3\xa5\x79\xa2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mallcoin Token",
        .unit = "MLC",
        .contract_address =
            "\xc7\x2e\xd4\x44\x5b\x3f\xe9\xf0\x86\x31\x06\xe3\x44\xe2\x41\x53\x0d\x33\x89\x06",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Melon Token",
        .unit = "MLN",
        .contract_address =
            "\xec\x67\x00\x5c\x4e\x49\x8e\xc7\xf5\x5e\x09\x2b\xd1\xd3\x5c\xbc\x47\xc9\x18\x92",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Moneynet Coin",
        .unit = "MNC",
        .contract_address =
            "\xba\xc7\xa1\x79\x83\x50\xcd\xf2\xdb\xfe\x0c\x21\x0c\x2c\x98\x61\x22\x3f\x4b\x31",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "minereum",
        .unit = "MNE",
        .contract_address =
            "\x1a\x95\xb2\x71\xb0\x53\x5d\x15\xfa\x49\x93\x2d\xab\xa3\x1b\xa6\x12\xb5\x29\x46",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Goldmint MNT Prelaunch Token",
        .unit = "MNTP",
        .contract_address =
            "\x83\xce\xe9\xe0\x86\xa7\x7e\x49\x2e\xe0\xbb\x93\xc2\xb0\x43\x7a\xd6\xfd\xec\xcc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Moss Coin",
        .unit = "MOC",
        .contract_address =
            "\x86\x5e\xc5\x8b\x06\xbf\x63\x05\xb8\x86\x79\x3a\xa2\x0a\x2d\xa3\x1d\x03\x4e\x68",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Modum Token",
        .unit = "MOD",
        .contract_address =
            "\x95\x7c\x30\xab\x04\x26\xe0\xc9\x3c\xd8\x24\x1e\x2c\x60\x39\x2d\x08\xc6\xac\x8e",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Molecular Future",
        .unit = "MOF",
        .contract_address =
            "\x65\x34\x30\x56\x0b\xe8\x43\xc4\xa3\xd1\x43\xd0\x11\x0e\x89\x6c\x2a\xb8\xac\x0d",
        .decimals = 16,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mithril Ore",
        .unit = "MORE",
        .contract_address =
            "\x50\x12\x62\x28\x1b\x2b\xa0\x43\xe2\xfb\xf1\x49\x04\x98\x06\x89\xcd\xdb\x0c\x78",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MOT",
        .unit = "MOT",
        .contract_address =
            "\x26\x3c\x61\x84\x80\xdb\xe3\x5c\x30\x0d\x8d\x5e\xcd\xa1\x9b\xbb\x98\x6a\xca\xed",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mozo Token",
        .unit = "MOZO",
        .contract_address =
            "\x44\xbf\x22\x94\x9f\x9c\xc8\x4b\x61\xb9\x32\x8a\x9d\x88\x5d\x1b\x5c\x80\x6b\x41",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mark",
        .unit = "MRK",
        .contract_address =
            "\xf4\x53\xb5\xb9\xd4\xe0\xb5\xc6\x2f\xfb\x25\x6b\xb2\x37\x8c\xc2\xbc\x8e\x8a\x89",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Marcelo",
        .unit = "MRL",
        .contract_address =
            "\x82\x12\x5a\xfe\x01\x81\x9d\xff\x15\x35\xd0\xd6\x27\x6d\x57\x04\x52\x91\xb6\xc0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Morpheus.Network",
        .unit = "MRPH",
        .contract_address =
            "\x7b\x0c\x06\x04\x34\x68\x46\x99\x67\xdb\xa2\x2d\x1a\xf3\x3d\x77\xd4\x40\x56\xc8",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mothership",
        .unit = "MSP",
        .contract_address =
            "\x68\xAA\x3F\x23\x2d\xA9\xbd\xC2\x34\x34\x65\x54\x57\x94\xef\x3e\xEa\x52\x09\xBD",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Medical Token Currency",
        .unit = "MTC",
        .contract_address =
            "\x90\x5e\x33\x7c\x6c\x86\x45\x26\x3d\x35\x21\x20\x5a\xa3\x7b\xf4\xd0\x34\xe7\x45",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MTC Mesh Network",
        .unit = "MTC",
        .contract_address =
            "\xdf\xdc\x0d\x82\xd9\x6f\x8f\xd4\x0c\xa0\xcf\xb4\xa2\x88\x95\x5b\xec\xec\x20\x88",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Monetha",
        .unit = "MTH",
        .contract_address =
            "\xaf\x4d\xce\x16\xda\x28\x77\xf8\xc9\xe0\x05\x44\xc9\x3b\x62\xac\x40\x63\x1f\x16",
        .decimals = 5,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Metal",
        .unit = "MTL",
        .contract_address =
            "\xF4\x33\x08\x93\x66\x89\x9D\x83\xa9\xf2\x6A\x77\x3D\x59\xec\x7e\xCF\x30\x35\x5e",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MedToken",
        .unit = "MTN",
        .contract_address =
            "\x41\xdb\xec\xc1\xcd\xc5\x51\x7c\x6f\x76\xf6\xa6\xe8\x36\xad\xbe\xe2\x75\x4d\xe3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ModulTrade Token",
        .unit = "MTRc",
        .contract_address =
            "\x1e\x49\xff\x77\xc3\x55\xa3\xe3\x8d\x66\x51\xce\x84\x04\xaf\x0e\x48\xc5\x39\x5f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MultiVAC",
        .unit = "MTV",
        .contract_address =
            "\x8a\xa6\x88\xab\x78\x9d\x18\x48\xd1\x31\xc6\x5d\x98\xce\xaa\x88\x75\xd9\x7e\xf1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Matryx",
        .unit = "MTX",
        .contract_address =
            "\x0a\xf4\x4e\x27\x84\x63\x72\x18\xdd\x1d\x32\xa3\x22\xd4\x4e\x60\x3a\x8f\x0c\x6a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MaverickChain",
        .unit = "MVC",
        .contract_address =
            "\xb1\x7d\xf9\xa3\xb0\x95\x83\xa9\xbd\xcf\x75\x7d\x63\x67\x17\x14\x76\xd4\xd8\xa3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mass Vehicle Ledger Token",
        .unit = "MVL",
        .contract_address =
            "\xa8\x49\xea\xae\x99\x4f\xb8\x6a\xfa\x73\x38\x2e\x9b\xd8\x8c\x2b\x6b\x18\xdc\x71",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RED MWAT",
        .unit = "MWAT",
        .contract_address =
            "\x64\x25\xc6\xbe\x90\x2d\x69\x2a\xe2\xdb\x75\x2b\x3c\x26\x8a\xfa\xdb\x09\x9d\x3b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MXCToken",
        .unit = "MXC",
        .contract_address =
            "\x5c\xa3\x81\xbb\xfb\x58\xf0\x09\x2d\xf1\x49\xbd\x3d\x24\x3b\x08\xb9\xa8\x38\x6e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Maximine Coin",
        .unit = "MXM",
        .contract_address =
            "\x8e\x76\x6f\x57\xf7\xd1\x6c\xa5\x0b\x4a\x0b\x90\xb8\x8f\x64\x68\xa0\x9b\x04\x39",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mysterium",
        .unit = "MYST",
        .contract_address =
            "\xa6\x45\x26\x4C\x56\x03\xE9\x6c\x3b\x0B\x07\x8c\xda\xb6\x87\x33\x79\x4B\x0A\x71",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NANJCOIN",
        .unit = "NANJ",
        .contract_address =
            "\xff\xe0\x2e\xe4\xc6\x9e\xdf\x1b\x34\x0f\xca\xd6\x4f\xbd\x6b\x37\xa7\xb9\xe2\x65",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nebulas",
        .unit = "NAS",
        .contract_address =
            "\x5d\x65\xD9\x71\x89\x5E\xdc\x43\x8f\x46\x5c\x17\xDB\x69\x92\x69\x8a\x52\x31\x8D",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NaviToken",
        .unit = "NAVI",
        .contract_address =
            "\x58\x80\x47\x36\x5d\xf5\xba\x58\x9f\x92\x36\x04\xaa\xc2\x3d\x67\x35\x55\xc6\x23",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Niobium",
        .unit = "NBC",
        .contract_address =
            "\x9f\x19\x56\x17\xfa\x8f\xba\xd9\x54\x0c\x5d\x11\x3a\x99\xa0\xa0\x17\x2a\xae\xdc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NeuroChain Clausius",
        .unit = "NCC",
        .contract_address =
            "\x5d\x48\xf2\x93\xba\xed\x24\x7a\x2d\x01\x89\x05\x8b\xa3\x7a\xa2\x38\xbd\x47\x25",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nectar",
        .unit = "NCT",
        .contract_address =
            "\x9e\x46\xa3\x8f\x5d\xaa\xbe\x86\x83\xe1\x07\x93\xb0\x67\x49\xee\xf7\xd7\x33\xd1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NEVERDIE",
        .unit = "NDC",
        .contract_address =
            "\xa5\x4d\xdc\x7b\x3c\xce\x7f\xc8\xb1\xe3\xfa\x02\x56\xd0\xdb\x80\xd2\xc1\x09\x70",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "nDEX",
        .unit = "NDX",
        .contract_address =
            "\x19\x66\xd7\x18\xa5\x65\x56\x6e\x8e\x20\x27\x92\x65\x8d\x7b\x5f\xf4\xec\xe4\x69",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ethfinex Nectar Token",
        .unit = "NEC",
        .contract_address =
            "\xcc\x80\xc0\x51\x05\x7b\x77\x4c\xd7\x50\x67\xdc\x48\xf8\x98\x7c\x4e\xb9\x7a\x5e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nerves",
        .unit = "NER",
        .contract_address =
            "\xee\x5d\xfb\x5d\xdd\x54\xea\x2f\xb9\x3b\x79\x6a\x8a\x1b\x83\xc3\xfe\x38\xe0\xe6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NEXT",
        .unit = "NET",
        .contract_address =
            "\xF2\x92\x26\x91\x45\x95\x05\x2a\x04\xF5\xAF\xbe\x64\x10\xD0\xC3\xeD\x70\x75\x48",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NimiqNetwork",
        .unit = "NET",
        .contract_address =
            "\xcf\xb9\x86\x37\xbc\xae\x43\xC1\x33\x23\xEA\xa1\x73\x1c\xED\x2B\x71\x69\x62\xfD",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Neumark",
        .unit = "NEU",
        .contract_address =
            "\xa8\x23\xe6\x72\x20\x06\xaf\xe9\x9e\x91\xc3\x0f\xf5\x29\x50\x52\xfe\x6b\x8e\x32",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nexo",
        .unit = "NEXO",
        .contract_address =
            "\xb6\x21\x32\xe3\x5a\x6c\x13\xee\x1e\xe0\xf8\x4d\xc5\xd4\x0b\xad\x8d\x81\x52\x06",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NEXT.exchange",
        .unit = "NEXT",
        .contract_address =
            "\x4e\x00\x5a\x76\x0e\x00\xe1\x7c\x49\x12\xa8\x07\x0e\xec\x04\x7c\xfe\xcb\xab\xbb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NAGA Coin",
        .unit = "NGC",
        .contract_address =
            "\x72\xdd\x4b\x6b\xd8\x52\xa3\xaa\x17\x2b\xe4\xd6\xc5\xa6\xdb\xec\x58\x8c\xf1\x31",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Autonio",
        .unit = "NIO",
        .contract_address =
            "\x55\x54\xe0\x4e\x76\x53\x3e\x1d\x14\xc5\x2f\x05\xbe\xef\x6c\x9d\x32\x9e\x1e\x30",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NKN",
        .unit = "NKN",
        .contract_address =
            "\x5c\xf0\x47\x16\xba\x20\x12\x7f\x1e\x22\x97\xad\xdc\xf4\xb5\x03\x50\x00\xc9\xeb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Numeraire",
        .unit = "NMR",
        .contract_address =
            "\x17\x76\xe1\xf2\x6f\x98\xb1\xa5\xdf\x9c\xd3\x47\x95\x3a\x26\xdd\x3c\xb4\x66\x71",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NOAHCOIN",
        .unit = "NOAH",
        .contract_address =
            "\x58\xa4\x88\x41\x82\xd9\xe8\x35\x59\x7f\x40\x5e\x5f\x25\x82\x90\xe4\x6a\xe7\xc2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "No BS Crypto",
        .unit = "NOBS",
        .contract_address =
            "\xf4\xfa\xea\x45\x55\x75\x35\x4d\x26\x99\xbc\x20\x9b\x0a\x65\xca\x99\xf6\x99\x82",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ChangeNOW",
        .unit = "NOW",
        .contract_address =
            "\xe9\xa9\x5d\x17\x5a\x5f\x4c\x93\x69\xf3\xb7\x42\x22\x40\x2e\xb1\xb8\x37\x69\x3b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nitro",
        .unit = "NOX",
        .contract_address =
            "\xec\x46\xf8\x20\x7d\x76\x60\x12\x45\x4c\x40\x8d\xe2\x10\xbc\xbc\x22\x43\xe7\x1c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NPER",
        .unit = "NPER",
        .contract_address =
            "\x4c\xe6\xb3\x62\xbc\x77\xa2\x49\x66\xdd\xa9\x07\x8f\x9c\xef\x81\xb3\xb8\x86\xa7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PlusCoin",
        .unit = "NPLC",
        .contract_address =
            "\x97\xfb\x6f\xc2\xad\x53\x20\x33\xaf\x97\x04\x3b\x56\x31\x31\xc5\x20\x4f\x8a\x35",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NapoleonX",
        .unit = "NPX",
        .contract_address =
            "\x28\xb5\xe1\x2c\xce\x51\xf1\x55\x94\xb0\xb9\x1d\x5b\x5a\xda\xa7\x0f\x68\x4a\x02",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pundi X Token",
        .unit = "NPXS",
        .contract_address =
            "\xa1\x5c\x7e\xbe\x1f\x07\xca\xf6\xbf\xf0\x97\xd8\xa5\x89\xfb\x8a\xc4\x9a\xe5\xb3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Neural Protocol",
        .unit = "NRP",
        .contract_address =
            "\x39\x18\xc4\x2f\x14\xf2\xeb\x11\x68\x36\x5f\x91\x1f\x63\xe5\x40\xe5\xa3\x06\xb5",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NeuroToken",
        .unit = "NTK",
        .contract_address =
            "\x69\xbe\xab\x40\x34\x38\x25\x3f\x13\xb6\xe9\x2d\xb9\x1f\x7f\xb8\x49\x25\x82\x63",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Netkoin",
        .unit = "NTK",
        .contract_address =
            "\x5d\x4d\x57\xcd\x06\xfa\x7f\xe9\x9e\x26\xfd\xc4\x81\xb4\x68\xf7\x7f\x05\x07\x3c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Fujinto",
        .unit = "NTO",
        .contract_address =
            "\x8a\x99\xed\x8a\x1b\x20\x49\x03\xee\x46\xe7\x33\xf2\xc1\x28\x6f\x6d\x20\xb1\x77",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Network",
        .unit = "NTWK",
        .contract_address =
            "\x22\x33\x79\x9e\xe2\x68\x3d\x75\xdf\xef\xac\xbc\xd2\xa2\x6c\x78\xd3\x4b\x47\x0d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nuggets",
        .unit = "NUG",
        .contract_address =
            "\x24\x5e\xf4\x7d\x4d\x05\x05\xec\xf3\xac\x46\x3f\x4d\x81\xf4\x1a\xde\x8f\x1f\xd1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nuls",
        .unit = "NULS",
        .contract_address =
            "\xb9\x13\x18\xf3\x5b\xdb\x26\x2e\x94\x23\xbc\x7c\x7c\x2a\x3a\x93\xdd\x93\xc9\x2c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nexxus",
        .unit = "NXX",
        .contract_address =
            "\x76\x27\xde\x4b\x93\x26\x3a\x6a\x75\x70\xb8\xda\xfa\x64\xba\xe8\x12\xe5\xc3\x94",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Nexium",
        .unit = "NxC",
        .contract_address =
            "\x45\xe4\x2D\x65\x9D\x9f\x94\x66\xcD\x5D\xF6\x22\x50\x60\x33\x14\x5a\x9b\x89\xBc",
        .decimals = 3,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OpenANX",
        .unit = "OAX",
        .contract_address =
            "\x70\x1c\x24\x4b\x98\x8a\x51\x3c\x94\x59\x73\xde\xfa\x05\xde\x93\x3b\x23\xfe\x1d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OceanToken",
        .unit = "OCEAN",
        .contract_address =
            "\x98\x5d\xd3\xd4\x2d\xe1\xe2\x56\xd0\x9e\x1c\x10\xf1\x12\xbc\xcb\x80\x15\xad\x41",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OCoin",
        .unit = "OCN",
        .contract_address =
            "\x40\x92\x67\x8e\x4e\x78\x23\x0f\x46\xa1\x53\x4c\x0f\xbc\x8f\xa3\x97\x80\x89\x2b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ODEM Token",
        .unit = "ODEM",
        .contract_address =
            "\xbf\x52\xf2\xab\x39\xe2\x6e\x09\x51\xd2\xa0\x2b\x49\xb7\x70\x2a\xbe\x30\x40\x6a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Origo",
        .unit = "OGO",
        .contract_address =
            "\xff\x0e\x5e\x01\x4c\xf9\x7e\x06\x15\xcb\x50\xf6\xf3\x9d\xa6\x38\x8e\x2f\xae\x6e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Online.io",
        .unit = "OIO",
        .contract_address =
            "\xa3\xef\xef\x1a\x1e\x3d\x1a\xd7\x2b\x9d\x0f\x42\x53\xe7\xc9\xc0\x84\xc2\xc0\x8b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OKB",
        .unit = "OKB",
        .contract_address =
            "\x75\x23\x1f\x58\xb4\x32\x40\xc9\x71\x8d\xd5\x8b\x49\x67\xc5\x11\x43\x42\xa8\x6c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Olive",
        .unit = "OLE",
        .contract_address =
            "\x9d\x92\x23\x43\x6d\xdd\x46\x6f\xc2\x47\xe9\xdb\xbd\x20\x20\x7e\x64\x0f\xef\x58",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OmiseGO",
        .unit = "OMG",
        .contract_address =
            "\xd2\x61\x14\xcd\x6E\xE2\x89\xAc\xcF\x82\x35\x0c\x8d\x84\x87\xfe\xdB\x8A\x0C\x07",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Omix",
        .unit = "OMX",
        .contract_address =
            "\xb5\xdb\xc6\xd3\xcf\x38\x00\x79\xdf\x3b\x27\x13\x56\x64\xb6\xbc\xf4\x5d\x18\x69",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BigONE Token",
        .unit = "ONE",
        .contract_address =
            "\x94\x65\x51\xdd\x05\xc5\xab\xd7\xcc\x80\x89\x27\x48\x02\x25\xce\x36\xd8\xc4\x75",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "HarmonyOne",
        .unit = "ONE",
        .contract_address =
            "\x79\x9a\x42\x02\xc1\x2c\xa9\x52\xcb\x31\x15\x98\xa0\x24\xc8\x0e\xd3\x71\xa4\x1e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Menlo One",
        .unit = "ONE",
        .contract_address =
            "\x4d\x80\x75\x09\xae\xce\x24\xc0\xfa\x5a\x10\x2b\x6a\x3b\x05\x9e\xc6\xe1\x43\x92",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "onG",
        .unit = "ONG",
        .contract_address =
            "\xd3\x41\xd1\x68\x0e\xee\xe3\x25\x5b\x8c\x4c\x75\xbc\xce\x7e\xb5\x7f\x14\x4d\xae",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "On.Live",
        .unit = "ONL",
        .contract_address =
            "\x68\x63\xbe\x0e\x7c\xf7\xce\x86\x0a\x57\x47\x60\xe9\x02\x0d\x51\x9a\x8b\xdc\x47",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ONOT",
        .unit = "ONOT",
        .contract_address =
            "\xb3\x1c\x21\x99\x59\xe0\x6f\x9a\xfb\xeb\x36\xb3\x88\xa4\xba\xd1\x3e\x80\x27\x25",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OPEN Chain",
        .unit = "OPENC",
        .contract_address =
            "\x9d\x86\xb1\xb2\x55\x4e\xc4\x10\xec\xcf\xfb\xf1\x11\xa6\x99\x49\x10\x11\x13\x40",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Opus",
        .unit = "OPT",
        .contract_address =
            "\x43\x55\xfC\x16\x0f\x74\x32\x8f\x9b\x38\x3d\xF2\xEC\x58\x9b\xB3\xdF\xd8\x2B\xa0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OptiToken",
        .unit = "OPTI",
        .contract_address =
            "\x83\x29\x04\x86\x39\x78\xb9\x48\x02\x12\x31\x06\xe6\xeb\x49\x1b\xdf\x0d\xf9\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Orbs",
        .unit = "ORBS",
        .contract_address =
            "\xff\x56\xcc\x6b\x1e\x6d\xed\x34\x7a\xa0\xb7\x67\x6c\x85\xab\x0b\x3d\x08\xb0\xfa",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Origami Network",
        .unit = "ORI",
        .contract_address =
            "\xd2\xfa\x8f\x92\xea\x72\xab\xb3\x5d\xbd\x6d\xec\xa5\x71\x73\xd2\x2d\xb2\xba\x49",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ORIUM",
        .unit = "ORM",
        .contract_address =
            "\xd5\x1e\x85\x26\x30\xde\xbc\x24\xe9\xe1\x04\x1a\x03\xd8\x0a\x01\x07\xf8\xef\x0c",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ormeus Coin",
        .unit = "ORME",
        .contract_address =
            "\xc9\x6d\xf9\x21\x00\x9b\x79\x0d\xff\xca\x41\x23\x75\x25\x1e\xd1\xa2\xb7\x5c\x60",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OriginSport Token",
        .unit = "ORS",
        .contract_address =
            "\xeb\x9a\x4b\x18\x58\x16\xc3\x54\xdb\x92\xdb\x09\xcc\x3b\x50\xbe\x60\xb9\x01\xb6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OSAToken",
        .unit = "OSA",
        .contract_address =
            "\xc6\xab\xf3\xc0\x93\x41\x74\x1a\xc6\x04\x1b\x0b\x08\x19\x57\x01\xbd\xfd\x46\x0c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Open Trading Network",
        .unit = "OTN",
        .contract_address =
            "\x88\x1e\xf4\x82\x11\x98\x2d\x01\xe2\xcb\x70\x92\xc9\x15\xe6\x47\xcd\x40\xd8\x5c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OwnData",
        .unit = "OWN",
        .contract_address =
            "\x17\x0b\x27\x5c\xed\x08\x9f\xff\xae\xbf\xe9\x27\xf4\x45\xa3\x50\xed\x91\x60\xdc",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Oxycoin",
        .unit = "OXY",
        .contract_address =
            "\x86\x9b\x1f\x57\x38\x0a\xe5\x01\xd3\x87\xb1\x92\x62\xef\xd3\xc0\xeb\x75\x01\xb0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PCHAIN",
        .unit = "PAI",
        .contract_address =
            "\xb9\xbb\x08\xab\x7e\x9f\xa0\xa1\x35\x6b\xd4\xa3\x9e\xc0\xca\x26\x7e\x03\xb0\xb3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PolicyPal Network",
        .unit = "PAL",
        .contract_address =
            "\xfe\xda\xe5\x64\x26\x68\xf8\x63\x6a\x11\x98\x7f\xf3\x86\xbf\xd2\x15\xf9\x42\xee",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pareto Network Token",
        .unit = "PARETO",
        .contract_address =
            "\xea\x5f\x88\xe5\x4d\x98\x2c\xbb\x0c\x44\x1c\xde\x4e\x79\xbc\x30\x5e\x5b\x43\xbc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Blockpass",
        .unit = "PASS",
        .contract_address =
            "\xee\x44\x58\xe0\x52\xb5\x33\xb1\xaa\xbd\x49\x3b\x5f\x8c\x4d\x85\xd7\xb2\x63\xdc",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PASS Token",
        .unit = "PASS",
        .contract_address =
            "\x77\x76\x1e\x63\xc0\x5a\xee\x66\x48\xfd\xae\xaa\x9b\x94\x24\x83\x51\xaf\x9b\xcd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PATRON",
        .unit = "PAT",
        .contract_address =
            "\xF3\xb3\xCa\xd0\x94\xB8\x93\x92\xfc\xE5\xfa\xFD\x40\xbC\x03\xb8\x0F\x2B\xc6\x24",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pangea Arbitration Token",
        .unit = "PAT",
        .contract_address =
            "\xBB\x1f\xA4\xFd\xEB\x34\x59\x73\x3b\xF6\x7E\xbC\x6f\x89\x30\x03\xfA\x97\x6a\x82",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Paxos Standard",
        .unit = "PAX",
        .contract_address =
            "\x8e\x87\x0d\x67\xf6\x60\xd9\x5d\x5b\xe5\x30\x38\x0d\x0e\xc0\xbd\x38\x82\x89\xe1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Paxos Gold",
        .unit = "PAXG",
        .contract_address =
            "\x45\x80\x48\x80\xDe\x22\x91\x3d\xAF\xE0\x9f\x49\x80\x84\x8E\xCE\x6E\xcb\xAf\x78",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TenXPay",
        .unit = "PAY",
        .contract_address =
            "\xB9\x70\x48\x62\x8D\xB6\xB6\x61\xD4\xC2\xaA\x83\x3e\x95\xDb\xe1\xA9\x05\xB2\x80",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pebbles",
        .unit = "PBL",
        .contract_address =
            "\x55\x64\x8d\xe1\x98\x36\x33\x85\x49\x13\x0b\x1a\xf5\x87\xf1\x6b\xea\x46\xf6\x6b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "POPCHAIN CASH",
        .unit = "PCH",
        .contract_address =
            "\xe3\xf4\xb4\xa5\xd9\x1e\x5c\xb9\x43\x5b\x94\x7f\x09\x0a\x31\x97\x37\x03\x63\x12",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Peculium",
        .unit = "PCL",
        .contract_address =
            "\x0f\x02\xe2\x77\x45\xe3\xb6\xe9\xe1\x31\x0d\x19\x46\x9e\x2b\x5d\x7b\x5e\xc9\x9a",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PDATA",
        .unit = "PDATA",
        .contract_address =
            "\x0d\xb0\x3b\x6c\xde\x0b\x2d\x42\x7c\x64\xa0\x4f\xea\xfd\x82\x59\x38\x36\x8f\x1f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Perlin",
        .unit = "PERL",
        .contract_address =
            "\xb5\xa7\x3f\x5f\xc8\xbb\xdb\xce\x59\xbf\xd0\x1c\xa8\xd3\x50\x62\xe0\xda\xd8\x01",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PayFair Token",
        .unit = "PFR",
        .contract_address =
            "\xb4\x14\x22\xd5\xa1\xd5\xd5\xc7\x3c\x22\x96\x86\x93\x5b\x40\xf8\x81\x50\x27\x85",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Puregold Token",
        .unit = "PGT",
        .contract_address =
            "\x9b\x3e\x94\x6e\x1a\x8e\xa0\x11\x2b\x14\x7a\xf4\xe6\xe0\x20\x75\x2f\x24\x46\xbc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PHI Token",
        .unit = "PHI",
        .contract_address =
            "\x13\xc2\xfa\xb6\x35\x4d\x37\x90\xd8\xec\xe4\xf0\xf1\xa3\x28\x0b\x4a\x25\xad\x96",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PIX",
        .unit = "PIX",
        .contract_address =
            "\x8e\xFF\xd4\x94\xeB\x69\x8c\xc3\x99\xAF\x62\x31\xfC\xcd\x39\xE0\x8f\xd2\x0B\x15",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PKG Token",
        .unit = "PKG",
        .contract_address =
            "\x02\xf2\xd4\xa0\x4e\x6e\x01\xac\xe8\x8b\xd2\xcd\x63\x28\x75\x54\x3b\x2e\xf5\x77",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Playkey",
        .unit = "PKT",
        .contract_address =
            "\x26\x04\xfa\x40\x6b\xe9\x57\xe5\x42\xbe\xb8\x9e\x67\x54\xfc\xde\x68\x15\xe8\x3f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PlayChip",
        .unit = "PLA",
        .contract_address =
            "\x01\x98\xf4\x6f\x52\x0f\x33\xcd\x43\x29\xbd\x4b\xe3\x80\xa2\x5a\x90\x53\x6c\xd5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Herocoin",
        .unit = "PLAY",
        .contract_address =
            "\xe4\x77\x29\x2f\x1b\x32\x68\x68\x7a\x29\x37\x61\x16\xb0\xed\x27\xa9\xc7\x61\x70",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Polybius",
        .unit = "PLBT",
        .contract_address =
            "\x0a\xff\xa0\x6e\x7f\xbe\x5b\xc9\xa7\x64\xc9\x79\xaa\x66\xe8\x25\x6a\x63\x1f\x02",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PILLAR",
        .unit = "PLR",
        .contract_address =
            "\xe3\x81\x85\x04\xc1\xb3\x2b\xf1\x55\x7b\x16\xc2\x38\xb2\xe0\x1f\xd3\x14\x9c\x17",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PlatonCoin",
        .unit = "PLTC",
        .contract_address =
            "\x0c\x6e\x8a\x83\x58\xcb\xde\x54\xf8\xe4\xcd\x7f\x07\xd5\xac\x38\xae\xc8\xc5\xa4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pluton",
        .unit = "PLU",
        .contract_address =
            "\xD8\x91\x2C\x10\x68\x1D\x8B\x21\xFd\x37\x42\x24\x4f\x44\x65\x8d\xBA\x12\x26\x4E",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PumaPay",
        .unit = "PMA",
        .contract_address =
            "\x84\x6c\x66\xcf\x71\xc4\x3f\x80\x40\x3b\x51\xfe\x39\x06\xb3\x59\x9d\x63\x33\x6f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Paymon",
        .unit = "PMNT",
        .contract_address =
            "\x81\xb4\xd0\x86\x45\xda\x11\x37\x4a\x03\x74\x9a\xb1\x70\x83\x6e\x4e\x53\x97\x67",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pinakion",
        .unit = "PNK",
        .contract_address =
            "\x93\xed\x3f\xbe\x21\x20\x7e\xc2\xe8\xf2\xd3\xc3\xde\x6e\x05\x8c\xb7\x3b\xc0\x4d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "POA ERC20 on Foundation",
        .unit = "POA20",
        .contract_address =
            "\x67\x58\xb7\xd4\x41\xa9\x73\x9b\x98\x55\x2b\x37\x37\x03\xd8\xd3\xd1\x4f\x9e\x62",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Po.et",
        .unit = "POE",
        .contract_address =
            "\x0e\x09\x89\xb1\xf9\xb8\xa3\x89\x83\xc2\xba\x80\x53\x26\x9c\xa6\x2e\xc9\xb1\x95",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ClearPoll",
        .unit = "POLL",
        .contract_address =
            "\x70\x5E\xE9\x6c\x1c\x16\x08\x42\xC9\x2c\x1a\xeC\xfC\xFf\xcc\xc9\xC4\x12\xe3\xD9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Polymath",
        .unit = "POLY",
        .contract_address =
            "\x99\x92\xec\x3c\xf6\xa5\x5b\x00\x97\x8c\xdd\xf2\xb2\x7b\xc6\x88\x2d\x88\xd1\xec",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PoSToken",
        .unit = "POS",
        .contract_address =
            "\xee\x60\x9f\xe2\x92\x12\x8c\xad\x03\xb7\x86\xdb\xb9\xbc\x26\x34\xcc\xdb\xe7\xfc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Posscoin",
        .unit = "POSS",
        .contract_address =
            "\x6b\x19\x3e\x10\x7a\x77\x39\x67\xbd\x82\x1b\xcf\x82\x18\xf3\x54\x8c\xfa\x25\x03",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PowerLedger",
        .unit = "POWR",
        .contract_address =
            "\x59\x58\x32\xf8\xfc\x6b\xf5\x9c\x85\xc5\x27\xfe\xc3\x74\x0a\x1b\x7a\x36\x12\x69",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PayPie",
        .unit = "PPP",
        .contract_address =
            "\xc4\x22\x09\xaC\xcC\x14\x02\x9c\x10\x12\xfB\x56\x80\xD9\x5f\xBd\x60\x36\xE2\xa0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Populous",
        .unit = "PPT",
        .contract_address =
            "\xd4\xfa\x14\x60\xf5\x37\xbb\x90\x85\xd2\x2c\x7b\xcc\xb5\xdd\x45\x0e\xf2\x8e\x3a",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Partner",
        .unit = "PRC",
        .contract_address =
            "\xca\xa0\x5e\x82\xbd\xcb\xa9\xe2\x5c\xd1\xa3\xbf\x1a\xfb\x79\x0c\x17\x58\x94\x3d",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Presearch",
        .unit = "PRE",
        .contract_address =
            "\x88\xa3\xe4\xf3\x5d\x64\xaa\xd4\x1a\x6d\x40\x30\xac\x9a\xfe\x43\x56\xcb\x84\xfa",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PRG",
        .unit = "PRG",
        .contract_address =
            "\x77\x28\xdF\xEF\x5a\xBd\x46\x86\x69\xEB\x7f\x9b\x48\xA7\xf7\x0a\x50\x1e\xD2\x9D",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Privatix",
        .unit = "PRIX",
        .contract_address =
            "\x3a\xdf\xc4\x99\x9f\x77\xd0\x4c\x83\x41\xba\xc5\xf3\xa7\x6f\x58\xdf\xf5\xb3\x7a",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Propy",
        .unit = "PRO",
        .contract_address =
            "\x22\x6b\xb5\x99\xa1\x2c\x82\x64\x76\xe3\xa7\x71\x45\x46\x97\xea\x52\xe9\xe2\x20",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pro",
        .unit = "PRO",
        .contract_address =
            "\x90\x41\xfe\x5b\x3f\xde\xa0\xf5\xe4\xaf\xdc\x17\xe7\x51\x80\x73\x8d\x87\x7a\x01",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Token Prometeus Network",
        .unit = "PROM",
        .contract_address =
            "\xfc\x82\xbb\x4b\xa8\x60\x45\xaf\x6f\x32\x73\x23\xa4\x6e\x80\x41\x2b\x91\xb2\x7d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PRASM",
        .unit = "PSM",
        .contract_address =
            "\x1a\x66\xe0\x9f\x7d\xcc\xc1\x0e\xae\x46\xe2\x7c\xfa\x6b\x8d\x44\xa5\x0d\xf1\xe7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Primas Token",
        .unit = "PST",
        .contract_address =
            "\x5d\x4a\xbc\x77\xb8\x40\x5a\xd1\x77\xd8\xac\x66\x82\xd5\x84\xec\xbf\xd4\x6c\xec",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Patientory",
        .unit = "PTOY",
        .contract_address =
            "\x8a\xe4\xbf\x2c\x33\xa8\xe6\x67\xde\x34\xb5\x49\x38\xb0\xcc\xd0\x3e\xb8\xcc\x06",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "PitisCoin",
        .unit = "PTS",
        .contract_address =
            "\x54\x0e\x5f\xff\x29\x3f\x52\x3a\xcd\x26\x29\x1b\x5b\xc7\xac\x57\x13\x99\x1f\xeb",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Proton Token",
        .unit = "PTT",
        .contract_address =
            "\x46\x89\xa4\xe1\x69\xeb\x39\xcc\x90\x78\xc0\x94\x0e\x21\xff\x1a\xa8\xa3\x9b\x9c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Pylon",
        .unit = "PYLNT",
        .contract_address =
            "\x77\x03\xc3\x5c\xff\xdc\x5c\xda\x8d\x27\xaa\x3d\xf2\xf9\xba\x69\x64\x54\x4b\x6e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Paycentos",
        .unit = "PYN",
        .contract_address =
            "\x01\x42\xc3\xb2\xfc\x51\x81\x9b\x5a\xf5\xdf\xc4\xaa\x52\xdf\x97\x22\x79\x08\x51",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QASH",
        .unit = "QASH",
        .contract_address =
            "\x61\x8e\x75\xac\x90\xb1\x2c\x60\x49\xba\x3b\x27\xf5\xd5\xf8\x65\x1b\x00\x37\xf6",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Quantum",
        .unit = "QAU",
        .contract_address =
            "\x67\x1a\xbb\xe5\xce\x65\x24\x91\x98\x53\x42\xe8\x54\x28\xeb\x1b\x07\xbc\x6c\x64",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Qubitica",
        .unit = "QBIT",
        .contract_address =
            "\x16\x02\xaf\x2c\x78\x2c\xc0\x3f\x92\x41\x99\x2e\x24\x32\x90\xfc\xcf\x73\xbb\x13",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "qiibeeToken",
        .unit = "QBX",
        .contract_address =
            "\x24\x67\xaa\x6b\x5a\x23\x51\x41\x6f\xd4\xc3\xde\xf8\x46\x2d\x84\x1f\xee\xec\xec",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QChi",
        .unit = "QCH",
        .contract_address =
            "\x68\x7b\xfc\x3e\x73\xf6\xaf\x55\xf0\xcc\xca\x84\x50\x11\x4d\x10\x7e\x78\x1a\x0e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QuickX Protocol",
        .unit = "QCX",
        .contract_address =
            "\xf9\xe5\xaf\x7b\x42\xd3\x1d\x51\x67\x7c\x75\xbb\xbd\x37\xc1\x98\x6e\xc7\x9a\xee",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Q DAO Governance token v1.0",
        .unit = "QDAO",
        .contract_address =
            "\x31\x66\xc5\x70\x93\x5a\x7d\x85\x54\xc8\xf4\xea\x79\x2f\xf9\x65\xd2\xef\xe1\xf2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QuarkChain Token",
        .unit = "QKC",
        .contract_address =
            "\xea\x26\xc4\xac\x16\xd4\xa5\xa1\x06\x82\x0b\xc8\xae\xe8\x5f\xd0\xb7\xb2\xb6\x64",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Quant",
        .unit = "QNT",
        .contract_address =
            "\x4a\x22\x0e\x60\x96\xb2\x5e\xad\xb8\x83\x58\xcb\x44\x06\x8a\x32\x48\x25\x46\x75",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QNTU",
        .unit = "QNTU",
        .contract_address =
            "\x42\x34\xf6\x3b\x1d\x20\x2f\x6c\x01\x6c\xa3\xb6\xa0\xd4\x1d\x7d\x85\xf1\x77\x16",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QRL",
        .unit = "QRL",
        .contract_address =
            "\x69\x7b\xea\xc2\x8b\x09\xe1\x22\xc4\x33\x2d\x16\x39\x85\xe8\xa7\x31\x21\xb9\x7f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Quantstamp",
        .unit = "QSP",
        .contract_address =
            "\x99\xea\x4d\xb9\xee\x77\xac\xd4\x0b\x11\x9b\xd1\xdc\x4e\x33\xe1\xc0\x70\xb8\x0d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QUINADS",
        .unit = "QUIN",
        .contract_address =
            "\x86\xe4\x45\x43\x16\x4d\x9b\x97\xb1\x4e\xf7\xf6\xf3\xab\x7b\xa6\x70\xca\xb3\x46",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QunQunCommunities",
        .unit = "QUN",
        .contract_address =
            "\x26\x4d\xc2\xde\xdc\xdc\xbb\x89\x75\x61\xa5\x7c\xba\x50\x85\xca\x41\x6f\xb7\xb4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Qurito",
        .unit = "QURO",
        .contract_address =
            "\x07\x6a\x93\xa4\x0b\xf9\xe0\xd2\x1d\x3f\x75\xdd\x1e\x05\x84\xdd\xbe\x0f\x9d\x1a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "QVT",
        .unit = "QVT",
        .contract_address =
            "\x11\x83\xf9\x2a\x56\x24\xd6\x8e\x85\xff\xb9\x17\x0f\x16\xbf\x04\x43\xb4\xc2\x42",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Revain",
        .unit = "R",
        .contract_address =
            "\x48\xf7\x75\xef\xbe\x4f\x5e\xce\x6e\x0d\xf2\xf7\xb5\x93\x2d\xf5\x68\x23\xb9\x90",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rublix",
        .unit = "RBLX",
        .contract_address =
            "\xfc\x2c\x4d\x8f\x95\x00\x2c\x14\xed\x0a\x7a\xa6\x51\x02\xca\xc9\xe5\x95\x3b\x5e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RipioCreditNetwork",
        .unit = "RCN",
        .contract_address =
            "\xf9\x70\xb8\xe3\x6e\x23\xf7\xfc\x3f\xd7\x52\xee\xa8\x6f\x8b\xe8\xd8\x33\x75\xa6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "realchain",
        .unit = "RCT",
        .contract_address =
            "\x13\xf2\x5c\xd5\x2b\x21\x65\x0c\xaa\x82\x25\xc9\x94\x23\x37\xd9\x14\xc9\xb0\x30",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Raiden",
        .unit = "RDN",
        .contract_address =
            "\x25\x5a\xa6\xdf\x07\x54\x0c\xb5\xd3\xd2\x97\xf0\xd0\xd4\xd8\x4c\xb5\x2b\xc8\xe6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Real Estate Asset Ledger",
        .unit = "REAL",
        .contract_address =
            "\x92\x14\xec\x02\xcb\x71\xcb\xa0\xad\xa6\x89\x6b\x8d\xa2\x60\x73\x6a\x67\xab\x10",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rebellious",
        .unit = "REBL",
        .contract_address =
            "\x5f\x53\xf7\xa8\x07\x56\x14\xb6\x99\xba\xad\x0b\xc2\xc8\x99\xf4\xba\xd8\xfb\xbf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RefToken",
        .unit = "REF",
        .contract_address =
            "\x89\x30\x35\x00\xa7\xab\xfb\x17\x8b\x27\x4f\xd8\x9f\x24\x69\xc2\x64\x95\x1e\x1f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "REMME",
        .unit = "REM",
        .contract_address =
            "\x83\x98\x4d\x61\x42\x93\x4b\xb5\x35\x79\x3a\x82\xad\xb0\xa4\x6e\xf0\xf6\x6b\x6d",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Republic",
        .unit = "REN",
        .contract_address =
            "\x40\x8e\x41\x87\x6c\xcc\xdc\x0f\x92\x21\x06\x00\xef\x50\x37\x26\x56\x05\x2a\x38",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Reputation",
        .unit = "REP",
        .contract_address =
            "\x19\x85\x36\x5e\x9f\x78\x35\x9a\x9B\x6A\xD7\x60\xe3\x24\x12\xf4\xa4\x45\xE8\x62",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Request",
        .unit = "REQ",
        .contract_address =
            "\x8f\x82\x21\xaf\xbb\x33\x99\x8d\x85\x84\xa2\xb0\x57\x49\xba\x73\xc3\x7a\x93\x8a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "imbrex",
        .unit = "REX",
        .contract_address =
            "\xf0\x5a\x93\x82\xA4\xC3\xF2\x9E\x27\x84\x50\x27\x54\x29\x3D\x88\xb8\x35\x10\x9C",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Refereum",
        .unit = "RFR",
        .contract_address =
            "\xd0\x92\x9d\x41\x19\x54\xc4\x74\x38\xdc\x1d\x87\x1d\xd6\x08\x1f\x5c\x5e\x14\x9c",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rusgas",
        .unit = "RGS",
        .contract_address =
            "\x4c\x38\x3b\xdc\xae\x52\xa6\xe1\xcb\x81\x0c\x76\xc7\x0d\x6f\x31\xa2\x49\xec\x9b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RHOC",
        .unit = "RHOC",
        .contract_address =
            "\x16\x82\x96\xbb\x09\xe2\x4a\x88\x80\x5c\xb9\xc3\x33\x56\x53\x6b\x98\x0d\x3f\xc5",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Etheriya",
        .unit = "RIYA",
        .contract_address =
            "\x0b\x17\x24\xcc\x9f\xda\x01\x86\x91\x1e\xf6\xa7\x59\x49\xe9\xc0\xd3\xf0\xf2\xf3",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RLC",
        .unit = "RLC",
        .contract_address =
            "\x60\x7F\x4C\x5B\xB6\x72\x23\x0e\x86\x72\x08\x55\x32\xf7\xe9\x01\x54\x4a\x73\x75",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RouletteToken",
        .unit = "RLT",
        .contract_address =
            "\xcc\xed\x5b\x82\x88\x08\x6b\xe8\xc3\x8e\x23\x56\x7e\x68\x4c\x37\x40\xbe\x4d\x48",
        .decimals = 10,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Relex",
        .unit = "RLX",
        .contract_address =
            "\x4a\x42\xd2\xc5\x80\xf8\x3d\xce\x40\x4a\xca\xd1\x8d\xab\x26\xdb\x11\xa1\x75\x0e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RightMesh Token",
        .unit = "RMESH",
        .contract_address =
            "\x8d\x56\x82\x94\x1c\xe4\x56\x90\x0b\x12\xd4\x7a\xc0\x6a\x88\xb4\x7c\x76\x4c\xe1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "OneRoot Network",
        .unit = "RNT",
        .contract_address =
            "\xff\x60\x3f\x43\x94\x6a\x3a\x28\xdf\x5e\x6a\x73\x17\x25\x55\xd8\xc8\xb0\x23\x86",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RNTB Token",
        .unit = "RNTB",
        .contract_address =
            "\x1f\xe7\x0b\xe7\x34\xe4\x73\xe5\x72\x1e\xa5\x7c\x8b\x5b\x01\xe6\xca\xa5\x26\x86",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RoBET - Truly Decentralized Sportsbetting",
        .unit = "ROBET",
        .contract_address =
            "\x23\x44\x87\x1f\x52\x3c\xbb\x28\xa4\xf6\x00\x45\x53\x11\x84\xcf\x1f\x03\xad\x24",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ROC",
        .unit = "ROC",
        .contract_address =
            "\x1b\xcb\xc5\x41\x66\xf6\xba\x14\x99\x34\x87\x0b\x60\x50\x61\x99\xb6\xc9\xdb\x6d",
        .decimals = 10,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ICE ROCK MINING",
        .unit = "ROCK2",
        .contract_address =
            "\xC1\x6b\x54\x2f\xf4\x90\xe0\x1f\xcc\x0D\xC5\x8a\x60\xe1\xEF\xdc\x3e\x35\x7c\xA6",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DICE",
        .unit = "ROL",
        .contract_address =
            "\x2e\x07\x1D\x29\x66\xAa\x7D\x8d\xEC\xB1\x00\x58\x85\xbA\x19\x77\xD6\x03\x8A\x65",
        .decimals = 16,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Robotina token",
        .unit = "ROX",
        .contract_address =
            "\x57\x4f\x84\x10\x8a\x98\xc5\x75\x79\x4f\x75\x48\x3d\x80\x1d\x1d\x5d\xc8\x61\xa5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rapids",
        .unit = "RPD",
        .contract_address =
            "\x4b\xf4\xf2\xea\x25\x8b\xf5\xcb\x69\xe9\xdc\x0d\xdb\x4a\x7a\x46\xa7\xc1\x0c\x53",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rocket Pool",
        .unit = "RPL",
        .contract_address =
            "\xb4\xef\xd8\x5c\x19\x99\x9d\x84\x25\x13\x04\xbd\xa9\x9e\x90\xb9\x23\x00\xbd\x93",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "REPME",
        .unit = "RPM",
        .contract_address =
            "\x49\x0c\x95\xbe\x16\x38\x4e\x1f\x28\xb9\xe8\x64\xe9\x8f\xfe\xcf\xcb\xff\x38\x6d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "AB-CHAIN RTB token",
        .unit = "RTB",
        .contract_address =
            "\xec\x49\x1c\x10\x88\xea\xe9\x92\xb7\xa2\x14\xef\xb0\xa2\x66\xad\x09\x27\xa7\x2a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rate3",
        .unit = "RTE",
        .contract_address =
            "\x43\x6f\x0f\x3a\x98\x20\x74\xc4\xa0\x50\x84\x48\x5d\x42\x14\x66\xa9\x94\xfe\x53",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rotharium",
        .unit = "RTH",
        .contract_address =
            "\x3f\xd8\xf3\x9a\x96\x2e\xfd\xa0\x49\x56\x98\x1c\x31\xab\x89\xfa\xb5\xfb\x8b\xc8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RUFF",
        .unit = "RUFF",
        .contract_address =
            "\xf2\x78\xc1\xca\x96\x90\x95\xff\xdd\xde\xd0\x20\x29\x0c\xf8\xb5\xc4\x24\xac\xe2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "RvT",
        .unit = "RVT",
        .contract_address =
            "\x3d\x1b\xa9\xbe\x9f\x66\xb8\xee\x10\x19\x11\xbc\x36\xd3\xfb\x56\x2e\xac\x22\x44",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rating",
        .unit = "Rating",
        .contract_address =
            "\xe8\x66\x3a\x64\xa9\x61\x69\xff\x4d\x95\xb4\x29\x9e\x7a\xe9\xa7\x6b\x90\x5b\x31",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sharpay",
        .unit = "S",
        .contract_address =
            "\x96\xb0\xbf\x93\x9d\x94\x60\x09\x5c\x15\x25\x1f\x71\xfd\xa1\x1e\x41\xdc\xbd\xdb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sai Stablecoin v1.0",
        .unit = "SAI",
        .contract_address =
            "\x89\xd2\x4a\x6b\x4c\xcb\x1b\x6f\xaa\x26\x25\xfe\x56\x2b\xdd\x9a\x23\x26\x03\x59",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Salt",
        .unit = "SALT",
        .contract_address =
            "\x41\x56\xD3\x34\x2D\x5c\x38\x5a\x87\xD2\x64\xF9\x06\x53\x73\x35\x92\x00\x05\x81",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SAN",
        .unit = "SAN",
        .contract_address =
            "\x7c\x5a\x0c\xe9\x26\x7e\xd1\x9b\x22\xf8\xca\xe6\x53\xf1\x98\xe3\xe8\xda\xf0\x98",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Social Activity Token",
        .unit = "SAT",
        .contract_address =
            "\xc5\x6b\x13\xeb\xbc\xff\xa6\x7c\xfb\x79\x79\xb9\x00\xb7\x36\xb3\xfb\x48\x0d\x78",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SiaCashCoin",
        .unit = "SCC",
        .contract_address =
            "\x74\xfd\x51\xa9\x8a\x4a\x1e\xcb\xef\x8c\xc4\x3b\xe8\x01\xcc\xe6\x30\xe2\x60\xbd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SOCIALL",
        .unit = "SCL",
        .contract_address =
            "\xd7\x63\x17\x87\xb4\xdc\xc8\x7b\x12\x54\xcf\xd1\xe5\xce\x48\xe9\x68\x23\xde\xe8",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Soma Community",
        .unit = "SCT",
        .contract_address =
            "\x63\xb9\x92\xe6\x24\x6d\x88\xf0\x7f\xc3\x5a\x05\x6d\x2c\x36\x5e\x6d\x44\x1a\x3d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Six Domain Asset",
        .unit = "SDA",
        .contract_address =
            "\x42\x12\xfe\xa9\xfe\xc9\x02\x36\xec\xc5\x1e\x41\xe2\x09\x6b\x16\xce\xb8\x45\x55",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SealToken",
        .unit = "SEAL",
        .contract_address =
            "\xd7\xb3\x66\x9c\x7d\x3e\x38\xab\x5a\x44\x13\x83\xd4\x1f\x25\xe0\x03\xe0\x21\x48",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sentinel Chain",
        .unit = "SENC",
        .contract_address =
            "\xa1\x3f\x07\x43\x95\x1b\x4f\x6e\x3e\x3a\xa0\x39\xf6\x82\xe1\x72\x79\xf5\x2b\xc3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sense",
        .unit = "SENSE",
        .contract_address =
            "\x67\x45\xfa\xb6\x80\x1e\x37\x6c\xd2\x4f\x03\x57\x2b\x9c\x9b\x0d\x4e\xdd\xdc\xcf",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SENTinel",
        .unit = "SENT",
        .contract_address =
            "\xa4\x4e\x51\x37\x29\x3e\x85\x5b\x1b\x7b\xc7\xe2\xc6\xf8\xcd\x79\x6f\xfc\xb0\x37",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Signals Network Token",
        .unit = "SGN",
        .contract_address =
            "\xb2\x13\x5a\xb9\x69\x5a\x76\x78\xdd\x59\x0b\x1a\x99\x6c\xb0\xf3\x7b\xcb\x07\x18",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SGPay",
        .unit = "SGP",
        .contract_address =
            "\x33\xc6\x23\xa2\xba\xaf\xeb\x8d\x15\xdf\xaf\x3c\xe4\x40\x95\xef\xec\x83\xd7\x2c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sugar Exchange",
        .unit = "SGR",
        .contract_address =
            "\xcb\x5a\x05\xbe\xf3\x25\x76\x13\xe9\x84\xc1\x7d\xbc\xf0\x39\x95\x2b\x6d\x88\x3f",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ShipChain SHIP",
        .unit = "SHIP",
        .contract_address =
            "\xe2\x5b\x0b\xba\x01\xdc\x56\x30\x31\x2b\x6a\x21\x92\x7e\x57\x80\x61\xa1\x3f\x55",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sharpe Platform Token",
        .unit = "SHP",
        .contract_address =
            "\xef\x24\x63\x09\x93\x60\xa0\x85\xf1\xf1\x0b\x07\x6e\xd7\x2e\xf6\x25\x49\x7a\x06",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Signals",
        .unit = "SIG",
        .contract_address =
            "\x68\x88\xa1\x6e\xa9\x79\x2c\x15\xa4\xdc\xf2\xf6\xc6\x23\xd0\x55\xc8\xed\xe7\x92",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Simmitri",
        .unit = "SIM",
        .contract_address =
            "\x75\x28\xe3\x04\x03\x76\xed\xd5\xdb\x82\x63\xdb\x2f\x5b\xd1\xbe\xd9\x14\x67\xfb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sakura Bloom",
        .unit = "SKB",
        .contract_address =
            "\x4a\xf3\x28\xc5\x29\x21\x70\x6d\xcb\x73\x9f\x25\x78\x62\x10\x49\x91\x69\xaf\xe6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Skychain Global Token",
        .unit = "SKCH",
        .contract_address =
            "\x70\xc6\x21\xf9\x49\xb6\x55\x6c\x45\x45\x70\x7a\x2d\x5d\x73\xa7\x76\xb9\x83\x59",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SKILLCOIN",
        .unit = "SKILL",
        .contract_address =
            "\x41\x7d\x6f\xee\xae\x8b\x2f\xcb\x73\xd1\x4d\x64\xbe\x7f\x19\x2e\x49\x43\x19\x78",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SkinCoin",
        .unit = "SKIN",
        .contract_address =
            "\x2b\xdc\x0d\x42\x99\x60\x17\xfc\xe2\x14\xb2\x16\x07\xa5\x15\xda\x41\xa9\xe0\xc5",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Skrumble Network V2",
        .unit = "SKM",
        .contract_address =
            "\x04\x8f\xe4\x9b\xe3\x2a\xdf\xc9\xed\x68\xc3\x7d\x32\xb5\xec\x9d\xf1\x7b\x36\x03",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Speed Mining Service",
        .unit = "SMS",
        .contract_address =
            "\x39\x01\x3f\x96\x1c\x37\x8f\x02\xc2\xb8\x2a\x6e\x1d\x31\xe9\x81\x27\x86\xfd\x9d",
        .decimals = 3,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SunContract",
        .unit = "SNC",
        .contract_address =
            "\xF4\x13\x41\x46\xAF\x2d\x51\x1D\xd5\xEA\x8c\xDB\x1C\x4A\xC8\x8C\x57\xD6\x04\x04",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Snetwork",
        .unit = "SNET",
        .contract_address =
            "\xff\x19\x13\x8b\x03\x9d\x93\x8d\xb4\x6b\xdd\xa0\x06\x7d\xc4\xba\x13\x2e\xc7\x1c",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SNGLS",
        .unit = "SNGLS",
        .contract_address =
            "\xae\xc2\xe8\x7e\x0a\x23\x52\x66\xd9\xc5\xad\xc9\xde\xb4\xb2\xe2\x9b\x54\xd0\x09",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SONM",
        .unit = "SNM",
        .contract_address =
            "\x98\x3f\x6d\x60\xdb\x79\xea\x8c\xa4\xeb\x99\x68\xc6\xaf\xf8\xcf\xa0\x4b\x3c\x63",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Snovio",
        .unit = "SNOV",
        .contract_address =
            "\xbd\xc5\xba\xc3\x9d\xbe\x13\x2b\x1e\x03\x0e\x89\x8a\xe3\x83\x00\x17\xd7\xd9\x69",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sonder",
        .unit = "SNR",
        .contract_address =
            "\x47\xd1\xa5\x9c\xbd\xd1\x9a\xee\x06\x0c\x85\x9c\x00\x09\x27\x7e\x24\x53\x28\xae",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "StatusNetwork",
        .unit = "SNT",
        .contract_address =
            "\x74\x4d\x70\xfd\xbe\x2b\xa4\xcf\x95\x13\x16\x26\x61\x4a\x17\x63\xdf\x80\x5b\x9e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Silent Notary Token",
        .unit = "SNTR",
        .contract_address =
            "\x28\x59\x02\x1e\xe7\xf2\xcb\x10\x16\x2e\x67\xf3\x3a\xf2\xd2\x27\x64\xb3\x1a\xff",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sentivate",
        .unit = "SNTVT",
        .contract_address =
            "\x78\x65\xaf\x71\xcf\x0b\x28\x8b\x4e\x7f\x65\x4f\x4f\x78\x51\xeb\x46\xa2\xb7\xf8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Synthetix Network Token",
        .unit = "SNX",
        .contract_address =
            "\xc0\x11\xa7\x24\x00\xe5\x8e\xcd\x99\xee\x49\x7c\xf8\x9e\x37\x75\xd4\xbd\x73\x2f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "All Sports Coin",
        .unit = "SOC",
        .contract_address =
            "\x2d\x0e\x95\xbd\x47\x95\xd7\xac\xe0\xda\x3c\x0f\xf7\xb7\x06\xa5\x97\x0e\xb9\xd3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Soniq Token",
        .unit = "SONIQ",
        .contract_address =
            "\x1c\x62\xac\xa2\xb7\x60\x5d\xb3\x60\x6e\xac\xda\x7b\xc6\x7a\x18\x57\xdd\xb8\xff",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SPANK",
        .unit = "SPANK",
        .contract_address =
            "\x42\xd6\x62\x2d\xec\xe3\x94\xb5\x49\x99\xfb\xd7\x3d\x10\x81\x23\x80\x6f\x6a\x18",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SPARTA",
        .unit = "SPARTA",
        .contract_address =
            "\x24\xae\xf3\xbf\x1a\x47\x56\x15\x00\xf9\x43\x0d\x74\xed\x40\x97\xc4\x7f\x51\xf2",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SpaceChain",
        .unit = "SPC",
        .contract_address =
            "\x80\x69\x08\x0a\x92\x28\x34\x46\x0c\x3a\x09\x2f\xb2\xc1\x51\x02\x24\xdc\x06\x6b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SPINDLE",
        .unit = "SPD",
        .contract_address =
            "\x1d\xea\x97\x9a\xe7\x6f\x26\x07\x18\x70\xf8\x24\x08\x8d\xa7\x89\x79\xeb\x91\xc8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SPF",
        .unit = "SPF",
        .contract_address =
            "\x85\x08\x93\x89\xc1\x4b\xd9\xc7\x7f\xc2\xb8\xf0\xc3\xd1\xdc\x33\x63\xbf\x06\xef",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SPHTX",
        .unit = "SPHTX",
        .contract_address =
            "\x38\x33\xdd\xa0\xae\xb6\x94\x7b\x98\xce\x45\x4d\x89\x36\x6c\xba\x8c\xc5\x55\x28",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sapien Network",
        .unit = "SPN",
        .contract_address =
            "\x20\xf7\xa3\xdd\xf2\x44\xdc\x92\x99\x97\x5b\x4d\xa1\xc3\x9f\x8d\x5d\x75\xf0\x5a",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Spendcoin",
        .unit = "SPND",
        .contract_address =
            "\xdd\xd4\x60\xbb\xd9\xf7\x98\x47\xea\x08\x68\x15\x63\xe8\xa9\x69\x68\x67\x21\x0c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SP8DE Token",
        .unit = "SPX",
        .contract_address =
            "\x05\xaa\xaa\x82\x9a\xfa\x40\x7d\x83\x31\x5c\xde\xd1\xd4\x5e\xb1\x60\x25\x91\x0c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SRCOIN",
        .unit = "SRCOIN",
        .contract_address =
            "\xef\x8c\xf7\x9c\x21\x63\x7f\xc6\xf9\x51\xbc\xac\x34\x89\x15\x50\x8a\x63\x9a\x41",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SIRIN",
        .unit = "SRN",
        .contract_address =
            "\x68\xd5\x7c\x9a\x1c\x35\xf6\x3e\x2c\x83\xee\x8e\x49\xa6\x4e\x9d\x70\x52\x8d\x25",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Sharder",
        .unit = "SS",
        .contract_address =
            "\xbb\xff\x86\x2d\x90\x6e\x34\x8e\x99\x46\xbf\xb2\x13\x2e\xcb\x15\x7d\xa3\xd4\xb4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "smartshare token",
        .unit = "SSP",
        .contract_address =
            "\x62\x4d\x52\x0b\xab\x2e\x4a\xd8\x39\x35\xfa\x50\x3f\xb1\x30\x61\x43\x74\xe8\x50",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Simple Token",
        .unit = "ST",
        .contract_address =
            "\x2c\x4e\x8f\x2d\x74\x61\x13\xd0\x69\x6c\xe8\x9b\x35\xf0\xd8\xbf\x88\xe0\xae\xca",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "FirstBlood",
        .unit = "ST",
        .contract_address =
            "\xaf\x30\xd2\xa7\xe9\x0d\x7d\xc3\x61\xc8\xc4\x58\x5e\x9b\xb7\xd2\xf6\xf1\x5b\xc7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "StarterCoin",
        .unit = "STAC",
        .contract_address =
            "\x9a\x00\x5c\x9a\x89\xbd\x72\xa4\xbd\x27\x72\x1e\x7a\x09\xa3\xc1\x1d\x2b\x03\xc4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "STACS",
        .unit = "STACS",
        .contract_address =
            "\x28\x67\x08\xf0\x69\x22\x59\x05\x19\x46\x73\x75\x5f\x12\x35\x9e\x6a\xff\x6f\xe1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Starbase",
        .unit = "STAR",
        .contract_address =
            "\xf7\x0a\x64\x2b\xd3\x87\xf9\x43\x80\xff\xb9\x04\x51\xc2\xc8\x1d\x4e\xb8\x2c\xbc",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "STK",
        .unit = "STK",
        .contract_address =
            "\xae\x73\xb3\x8d\x1c\x9a\x8b\x27\x41\x27\xec\x30\x16\x0a\x49\x27\xc4\xd7\x18\x24",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Storj",
        .unit = "STORJ",
        .contract_address =
            "\xb6\x4e\xf5\x1c\x88\x89\x72\xc9\x08\xcf\xac\xf5\x9b\x47\xc1\xaf\xbc\x0a\xb8\xac",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Storm",
        .unit = "STORM",
        .contract_address =
            "\xd0\xa4\xb8\x94\x6c\xb5\x2f\x06\x61\x27\x3b\xfb\xc6\xfd\x0e\x0c\x75\xfc\x64\x33",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "STPT",
        .unit = "STPT",
        .contract_address =
            "\xde\x7d\x85\x15\x7d\x97\x14\xea\xdf\x59\x50\x45\xcc\x12\xca\x4a\x5f\x3e\x2a\xdb",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Storiqa",
        .unit = "STQ",
        .contract_address =
            "\x5c\x3a\x22\x85\x10\xd2\x46\xb7\x8a\x37\x65\xc2\x02\x21\xcb\xf3\x08\x2b\x44\xa4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Staker",
        .unit = "STR",
        .contract_address =
            "\xBA\xE2\x35\x82\x3D\x72\x55\xD9\xD4\x86\x35\xcE\xd4\x73\x52\x27\x24\x4C\xd5\x83",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "StarCredits",
        .unit = "STRC",
        .contract_address =
            "\x46\x49\x24\x73\x75\x5e\x8d\xf9\x60\xf8\x03\x48\x77\xf6\x17\x32\xd7\x18\xce\x96",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Student Coin",
        .unit = "STU",
        .contract_address =
            "\x03\x71\xa8\x2e\x4a\x9d\x0a\x43\x12\xf3\xee\x2a\xc9\xc6\x95\x85\x12\x89\x13\x72",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Stox",
        .unit = "STX",
        .contract_address =
            "\x00\x6b\xea\x43\xba\xa3\xf7\xa6\xf7\x65\xf1\x4f\x10\xa1\xa1\xb0\x83\x34\xef\x45",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Substratum",
        .unit = "SUB",
        .contract_address =
            "\x8d\x75\x95\x9f\x1e\x61\xec\x25\x71\xaa\x72\x79\x82\x37\x10\x1f\x08\x4d\xe6\x3a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Suretly",
        .unit = "SUR",
        .contract_address =
            "\xe1\x20\xc1\xec\xbf\xdf\xea\x7f\x0a\x8f\x0e\xe3\x00\x63\x49\x1e\x8c\x26\xfe\xdf",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SURETY Token",
        .unit = "SURE",
        .contract_address =
            "\x95\x38\x2a\xc8\x2e\x88\x6a\x36\x7b\xac\x9e\x1e\x23\xbe\xab\xe5\x69\xbc\xfe\xd8",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "savedroid",
        .unit = "SVD",
        .contract_address =
            "\xbd\xeb\x4b\x83\x25\x1f\xb1\x46\x68\x7f\xa1\x9d\x1c\x66\x0f\x99\x41\x1e\xef\xe3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Scanetchain",
        .unit = "SWC",
        .contract_address =
            "\xad\xf8\xb8\x05\x06\x39\xb6\x23\x69\x15\xf7\x51\x6d\x69\xde\x71\x46\x72\xf0\xbf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SwftCoin",
        .unit = "SWFTC",
        .contract_address =
            "\x0b\xb2\x17\xe4\x0f\x8a\x5c\xb7\x9a\xdf\x04\xe1\xaa\xb6\x0e\x5a\xbd\x0d\xfc\x1e",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SWARM",
        .unit = "SWM",
        .contract_address =
            "\x35\x05\xf4\x94\xc3\xf0\xfe\xd0\xb5\x94\xe0\x1f\xa4\x1d\xd3\x96\x76\x45\xca\x39",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SwarmCity",
        .unit = "SWT",
        .contract_address =
            "\xb9\xe7\xf8\x56\x8e\x08\xd5\x65\x9f\x5d\x29\xc4\x99\x71\x73\xd8\x4c\xdf\x26\x07",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Spectre.ai D-Token",
        .unit = "SXDT",
        .contract_address =
            "\x12\xb3\x06\xfa\x98\xf4\xcb\xb8\xd4\x45\x7f\xdf\xf3\xa0\xa0\xa5\x6f\x07\xcc\xdf",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Swipe",
        .unit = "SXP",
        .contract_address =
            "\x8c\xe9\x13\x7d\x39\x32\x6a\xd0\xcd\x64\x91\xfb\x5c\xc0\xcb\xa0\xe0\x89\xb6\xa9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Spectre.ai U-Token",
        .unit = "SXUT",
        .contract_address =
            "\x2c\x82\xc7\x3d\x5b\x34\xaa\x01\x59\x89\x46\x2b\x29\x48\xcd\x61\x6a\x37\x64\x1f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SeeleToken",
        .unit = "Seele",
        .contract_address =
            "\xb1\xee\xf1\x47\x02\x8e\x9f\x48\x0d\xbc\x5c\xca\xa3\x27\x7d\x41\x7d\x1b\x85\xf0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Soarcoin",
        .unit = "Soar",
        .contract_address =
            "\xD6\x59\x60\xFA\xcb\x8E\x4a\x2d\xFc\xb2\xC2\x21\x2c\xb2\xe4\x4a\x02\xe2\xa5\x7E",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TAAS",
        .unit = "TAAS",
        .contract_address =
            "\xe7\x77\x5a\x6e\x9b\xcf\x90\x4e\xb3\x9d\xa2\xb6\x8c\x5e\xfb\x4f\x93\x60\xe0\x8c",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Talao",
        .unit = "TALAO",
        .contract_address =
            "\x1d\x4c\xcc\x31\xda\xb6\xea\x20\xf4\x61\xd3\x29\xa0\x56\x2c\x1c\x58\x41\x25\x15",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Lamden Tau",
        .unit = "TAU",
        .contract_address =
            "\xc2\x7a\x2f\x05\xfa\x57\x7a\x83\xba\x0f\xdb\x4c\x38\x44\x3c\x07\x18\x35\x65\x01",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tokenbox",
        .unit = "TBX",
        .contract_address =
            "\x3a\x92\xbd\x39\x6a\xef\x82\xaf\x98\xeb\xc0\xaa\x90\x30\xd2\x5a\x23\xb1\x1c\x6b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ThoreCash",
        .unit = "TCH",
        .contract_address =
            "\x99\x72\xa0\xf2\x41\x94\x44\x7e\x73\xa7\xe8\xb6\xcd\x26\xa5\x2e\x02\xdd\xfa\xd5",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TokenClub Token",
        .unit = "TCT",
        .contract_address =
            "\x48\x24\xa7\xb6\x4e\x39\x66\xb0\x13\x3f\x4f\x4f\xfb\x1b\x9d\x6b\xeb\x75\xff\xf7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TrueDeck",
        .unit = "TDP",
        .contract_address =
            "\x5b\x11\xaa\xcb\x6b\xdd\xb9\xff\xab\x90\x8f\xdc\xe7\x39\xbf\x4a\xed\x55\x43\x27",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TEAM",
        .unit = "TEAM",
        .contract_address =
            "\x1c\x79\xab\x32\xc6\x6a\xca\xa1\xe9\xe8\x19\x52\xb8\xaa\xa5\x81\xb4\x3e\x54\xe7",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Telcoin",
        .unit = "TEL",
        .contract_address =
            "\x85\xe0\x76\x36\x1c\xc8\x13\xa9\x08\xff\x67\x2f\x9b\xad\x15\x41\x47\x44\x02\xb2",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tokenomy",
        .unit = "TEN",
        .contract_address =
            "\xdd\x16\xec\x0f\x66\xe5\x4d\x45\x3e\x67\x56\x71\x3e\x53\x33\x55\x98\x90\x40\xe4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "20-footEqvUnit",
        .unit = "TEU",
        .contract_address =
            "\xee\xac\x3f\x8d\xa1\x6b\xb0\x48\x5a\x4a\x11\xc5\x12\x8b\x05\x18\xda\xc8\x14\x48",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TE-FOOD",
        .unit = "TFD",
        .contract_address =
            "\xe5\xf1\x66\xc0\xd8\x87\x2b\x68\x79\x00\x61\x31\x7b\xb6\xcc\xa0\x45\x82\xc9\x12",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TrueFlip",
        .unit = "TFL",
        .contract_address =
            "\xa7\xf9\x76\xc3\x60\xeb\xbe\xd4\x46\x5c\x28\x55\x68\x4d\x1a\xae\x52\x71\xef\xa9",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TGAME",
        .unit = "TGAME",
        .contract_address =
            "\xf8\xe0\x6e\x4e\x4a\x80\x28\x7f\xdc\xa5\xb0\x2d\xcc\xec\xaa\x9d\x09\x54\x84\x0f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Target Coin",
        .unit = "TGT",
        .contract_address =
            "\xac\x3d\xa5\x87\xea\xc2\x29\xc9\x89\x6d\x91\x9a\xbc\x23\x5c\xa4\xfd\x7f\x72\xc1",
        .decimals = 1,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Theta Token",
        .unit = "THETA",
        .contract_address =
            "\x38\x83\xf5\xe1\x81\xfc\xca\xf8\x41\x0f\xa6\x1e\x12\xb5\x9b\xad\x96\x3f\xb6\x45",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Themis Token",
        .unit = "THM",
        .contract_address =
            "\xf1\xdd\x59\x64\xea\xbc\xc6\xe8\x62\x30\xfa\x6f\x22\x26\x77\xcf\xda\xaf\x9f\x0e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ThriveToken",
        .unit = "THRT",
        .contract_address =
            "\x4f\x27\x05\x3f\x32\xed\xa8\xaf\x84\x95\x64\x37\xbc\x00\xe5\xff\xa7\x00\x32\x87",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Thingschain",
        .unit = "TIC",
        .contract_address =
            "\x72\x43\x0a\x61\x2a\xdc\x00\x7c\x50\xe3\xb6\x94\x6d\xbb\x1b\xb0\xfd\x31\x01\xd1",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TieToken",
        .unit = "TIE",
        .contract_address =
            "\x99\x99\x67\xe2\xec\x8a\x74\xb7\xc8\xe9\xdb\x19\xe0\x39\xd9\x20\xb3\x1d\x39\xd0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tigereum",
        .unit = "TIG",
        .contract_address =
            "\xee\xe2\xd0\x0e\xb7\xde\xb8\xdd\x69\x24\x18\x7f\x5a\xa3\x49\x6b\x7d\x06\xe6\x2a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TIME",
        .unit = "TIME",
        .contract_address =
            "\x65\x31\xf1\x33\xe6\xde\xeb\xe7\xf2\xdc\xe5\xa0\x44\x1a\xa7\xef\x33\x0b\x4e\x53",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Trade Token X",
        .unit = "TIOx",
        .contract_address =
            "\xd9\x47\xb0\xce\xab\x2a\x88\x85\x86\x6b\x9a\x04\xa0\x6a\xe9\x9d\xe8\x52\xa3\xd4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Blocktix",
        .unit = "TIX",
        .contract_address =
            "\xea\x1f\x34\x6f\xaf\x02\x3f\x97\x4e\xb5\xad\xaf\x08\x8b\xbc\xdf\x02\xd7\x61\xf4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tokia",
        .unit = "TKA",
        .contract_address =
            "\xda\xe1\xba\xf2\x49\x96\x4b\xc4\xb6\xac\x98\xc3\x12\x2f\x0e\x3e\x78\x5f\xd2\x79",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TokenCard",
        .unit = "TKN",
        .contract_address =
            "\xaa\xaf\x91\xd9\xb9\x0d\xf8\x00\xdf\x4f\x55\xc2\x05\xfd\x69\x89\xc9\x77\xe7\x3a",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TOKPIE",
        .unit = "TKP",
        .contract_address =
            "\xd3\x16\x95\xa1\xd3\x5e\x48\x92\x52\xce\x57\xb1\x29\xfd\x4b\x1b\x05\xe6\xac\xac",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TKRToken",
        .unit = "TKR",
        .contract_address =
            "\xb4\x5a\x50\x54\x5b\xee\xab\x73\xf3\x8f\x31\xe5\x97\x37\x68\xc4\x21\x80\x5e\x5e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Traxia Membership Token",
        .unit = "TMT",
        .contract_address =
            "\x32\x09\xf9\x8b\xeb\xf0\x14\x9b\x76\x9c\xe2\x6d\x71\xf7\xae\xa8\xe4\x35\xef\xea",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Transcodium",
        .unit = "TNS",
        .contract_address =
            "\xb0\x28\x07\x43\xb4\x4b\xf7\xdb\x4b\x6b\xe4\x82\xb2\xba\x7b\x75\xe5\xda\x09\x6c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tierion Network Token",
        .unit = "TNT",
        .contract_address =
            "\x08\xf5\xa9\x23\x5b\x08\x17\x3b\x75\x69\xf8\x36\x45\xd2\xc7\xfb\x55\xe8\xcc\xd8",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tolar Token",
        .unit = "TOL",
        .contract_address =
            "\xd0\x7d\x9f\xe2\xd2\xcc\x06\x70\x15\xe2\xb4\x91\x7d\x24\x93\x38\x04\xf4\x2c\xfa",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TOP Network",
        .unit = "TOP",
        .contract_address =
            "\xdc\xd8\x59\x14\xb8\xae\x28\xc1\xe6\x2f\x1c\x48\x8e\x1d\x96\x8d\x5a\xaf\xfe\x2b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Trace",
        .unit = "TRAC",
        .contract_address =
            "\xaa\x7a\x9c\xa8\x7d\x36\x94\xb5\x75\x5f\x21\x3b\x5d\x04\x09\x4b\x8d\x0f\x0a\x6f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TrakInvest Token",
        .unit = "TRAK",
        .contract_address =
            "\x12\x75\x95\x12\xd3\x26\x30\x3b\x45\xf1\xce\xc8\xf7\xb6\xfd\x96\xf3\x87\x77\x8e",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tellor Tributes",
        .unit = "TRB",
        .contract_address =
            "\x0b\xa4\x5a\x8b\x5d\x55\x75\x93\x5b\x81\x58\xa8\x8c\x63\x1e\x9f\x9c\x95\xa2\xe5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tracto",
        .unit = "TRCT",
        .contract_address =
            "\x30\xce\xCB\x54\x61\xA4\x49\xA9\x00\x81\xF5\xa5\xF5\x5d\xb4\xe0\x48\x39\x7B\xAB",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Trident Group",
        .unit = "TRDT",
        .contract_address =
            "\x33\xf9\x0d\xee\x07\xc6\xe8\xb9\x68\x2d\xd2\x0f\x73\xe6\xc3\x58\xb2\xed\x0f\x03",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tripio",
        .unit = "TRIO",
        .contract_address =
            "\x8b\x40\x76\x11\x42\xb9\xaa\x6d\xc8\x96\x4e\x61\xd0\x58\x59\x95\x42\x5c\x3d\x94",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Trustcoin",
        .unit = "TRST",
        .contract_address =
            "\xcb\x94\xbe\x6f\x13\xa1\x18\x2e\x4a\x4b\x61\x40\xcb\x7b\xf2\x02\x5d\x28\xe4\x1b",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TRONCLASSIC",
        .unit = "TRXC",
        .contract_address =
            "\xad\x5f\xe5\xb0\xb8\xec\x8f\xf4\x56\x52\x04\x99\x0e\x44\x05\xb2\xda\x11\x7d\x8e",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TESLA",
        .unit = "TSL",
        .contract_address =
            "\x03\x80\x6c\xe5\xef\x69\xbd\x97\x80\xed\xfb\x04\xc2\x9d\xa1\xf2\x3d\xb9\x62\x94",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TTC Protocol (TTC)",
        .unit = "TTC",
        .contract_address =
            "\x93\x89\x43\x48\x52\xb9\x4b\xba\xd4\xc8\xaf\xed\x5b\x7b\xdb\xc5\xff\x0c\x22\x75",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TaTaTu",
        .unit = "TTU",
        .contract_address =
            "\x9c\xda\x8a\x60\xdd\x5a\xfa\x15\x6c\x95\xbd\x97\x44\x28\xd9\x1a\x08\x12\xe0\x54",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TrueUSD",
        .unit = "TUSD",
        .contract_address =
            "\x00\x00\x00\x00\x00\x08\x5d\x47\x80\xB7\x31\x19\xb6\x44\xAE\x5e\xcd\x22\xb3\x76",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "TravelNote",
        .unit = "TVNT",
        .contract_address =
            "\x56\x35\xdd\xea\xbf\x9c\xdd\xa6\x86\x99\x5f\xe9\x0b\xeb\x54\x11\x83\x15\x63\xfc",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Typerium",
        .unit = "TYPE",
        .contract_address =
            "\xea\xf6\x1f\xc1\x50\xcd\x5c\x3b\xea\x75\x74\x4e\x83\x0d\x91\x6e\x60\xea\x5a\x9f",
        .decimals = 4,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UBCoin",
        .unit = "UBC",
        .contract_address =
            "\x2d\x3e\x7d\x48\x70\xa5\x1b\x91\x89\x19\xe7\xb8\x51\xfe\x19\x98\x3e\x4c\x38\xd5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UBEX Token",
        .unit = "UBEX",
        .contract_address =
            "\x67\x04\xb6\x73\xc7\x0d\xe9\xbf\x74\xc8\xfb\xa4\xb4\xbd\x74\x8f\x0e\x21\x90\xe1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UniBright",
        .unit = "UBT",
        .contract_address =
            "\x84\x00\xd9\x4a\x5c\xb0\xfa\x0d\x04\x1a\x37\x88\xe3\x95\x28\x5d\x61\xc9\xee\x5e",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UCASH",
        .unit = "UCASH",
        .contract_address =
            "\x92\xe5\x2a\x1a\x23\x5d\x9a\x10\x3d\x97\x09\x01\x06\x6c\xe9\x10\xaa\xce\xfd\x37",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UChain Token",
        .unit = "UCN",
        .contract_address =
            "\xaa\xf3\x70\x55\x18\x8f\xee\xe4\x86\x9d\xe6\x34\x64\x93\x7e\x68\x3d\x61\xb2\xa1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UselessEthereumToken",
        .unit = "UET",
        .contract_address =
            "\x27\xf7\x06\xed\xde\x3a\xD9\x52\xEF\x64\x7D\xd6\x7E\x24\xe3\x8C\xD0\x80\x3D\xD6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Upfiring",
        .unit = "UFR",
        .contract_address =
            "\xea\x09\x7a\x2b\x1d\xb0\x06\x27\xb2\xfa\x17\x46\x0a\xd2\x60\xc0\x16\x01\x69\x77",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UltrainGas",
        .unit = "UGAS",
        .contract_address =
            "\x87\x16\xfc\x5d\xa0\x09\xd3\xa2\x08\xf0\x17\x8b\x63\x7a\x50\xf4\xef\x42\x40\x0f",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UnlimitedIP Token",
        .unit = "UIP",
        .contract_address =
            "\x42\x90\x56\x3c\x2d\x7c\x25\x5b\x5e\xec\x87\xf2\xd3\xbd\x10\x38\x9f\x99\x1d\x68",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UnikoinGold",
        .unit = "UKG",
        .contract_address =
            "\x24\x69\x27\x91\xbc\x44\x4c\x5c\xd0\xb8\x1e\x3c\xbc\xab\xa4\xb0\x4a\xcd\x1f\x3b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ultiledger",
        .unit = "ULT",
        .contract_address =
            "\xe8\x84\xcc\x27\x95\xb9\xc4\x5b\xee\xac\x06\x07\xda\x95\x39\xfd\x57\x1c\xcf\x85",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UpToken",
        .unit = "UP",
        .contract_address =
            "\x6b\xa4\x60\xab\x75\xcd\x2c\x56\x34\x3b\x35\x17\xff\xeb\xa6\x07\x48\x65\x4d\x26",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "SENTINEL PROTOCOL",
        .unit = "UPP",
        .contract_address =
            "\xc8\x6d\x05\x48\x09\x62\x34\x32\x21\x0c\x10\x7a\xf2\xe3\xf6\x19\xdc\xfb\xf6\x52",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Uquid Coin",
        .unit = "UQC",
        .contract_address =
            "\xd0\x1d\xb7\x3e\x04\x78\x55\xef\xb4\x14\xe6\x20\x20\x98\xc4\xbe\x4c\xd2\x42\x3b",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "USD Coin",
        .unit = "USDC",
        .contract_address =
            "\xa0\xb8\x69\x91\xc6\x21\x8b\x36\xc1\xd1\x9d\x4a\x2e\x9e\xb0\xce\x36\x06\xeb\x48",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "USDQ Stablecoin by Q DAO v1.0",
        .unit = "USDQ",
        .contract_address =
            "\x49\x54\xdb\x63\x91\xf4\xfe\xb5\x46\x8b\x6b\x94\x3d\x49\x35\x35\x35\x96\xae\xc9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Tether USD",
        .unit = "USDT",
        .contract_address =
            "\xda\xc1\x7f\x95\x8d\x2e\xe5\x23\xa2\x20\x62\x06\x99\x45\x97\xc1\x3d\x83\x1e\xc7",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UTRUST",
        .unit = "UTK",
        .contract_address =
            "\x70\xa7\x28\x33\xd6\xbf\x7f\x50\x8c\x82\x24\xce\x59\xea\x1e\xf3\xd0\xea\x3a\x38",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UTN-P: Universa Token",
        .unit = "UTNP",
        .contract_address =
            "\x9e\x33\x19\x63\x6e\x21\x26\xe3\xc0\xbc\x9e\x31\x34\xae\xc5\xe1\x50\x8a\x46\xc7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "United Traders Token",
        .unit = "UTT",
        .contract_address =
            "\x16\xf8\x12\xbe\x7f\xff\x02\xca\xf6\x62\xb8\x5d\x5d\x58\xa5\xda\x65\x72\xd4\xdf",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "UNetworkToken",
        .unit = "UUU",
        .contract_address =
            "\x35\x43\x63\x8e\xd4\xa9\x00\x6e\x48\x40\xb1\x05\x94\x42\x71\xbc\xea\x15\x60\x5d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ValueCyberToken",
        .unit = "VCT",
        .contract_address =
            "\x97\x46\x95\x3f\x5b\x13\x24\xa7\x81\x32\x89\x5c\xfd\x26\x3f\x41\x7b\x0f\xaa\xe3",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VeriDocGlobal",
        .unit = "VDG",
        .contract_address =
            "\x57\xC7\x5E\xCC\xc8\x55\x71\x36\xD3\x26\x19\xa1\x91\xfB\xCD\xc8\x85\x60\xd7\x11",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Vodi X",
        .unit = "VDX",
        .contract_address =
            "\x91\xe6\x4f\x39\xc1\xfe\x14\x49\x2e\x8f\xdf\x5a\x8b\x0f\x30\x5b\xd2\x18\xc8\xa1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BLOCKv",
        .unit = "VEE",
        .contract_address =
            "\x34\x0d\x2b\xde\x5e\xb2\x8c\x1e\xed\x91\xb2\xf7\x90\x72\x3e\x3b\x16\x06\x13\xb7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VeChain",
        .unit = "VEN",
        .contract_address =
            "\xd8\x50\x94\x2e\xf8\x81\x1f\x2a\x86\x66\x92\xa6\x23\x01\x1b\xde\x52\xa4\x62\xc1",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Veritaseum",
        .unit = "VERI",
        .contract_address =
            "\x8f\x34\x70\xA7\x38\x8c\x05\xeE\x4e\x7A\xF3\xd0\x1D\x8C\x72\x2b\x0F\xF5\x23\x74",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Vestchain",
        .unit = "VEST",
        .contract_address =
            "\x37\xf0\x4d\x2c\x3a\xe0\x75\xfa\xd5\x48\x3b\xb9\x18\x49\x1f\x65\x6b\x12\xbd\xb6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VIB",
        .unit = "VIB",
        .contract_address =
            "\x2C\x97\x4B\x2d\x0B\xA1\x71\x6E\x64\x4c\x1F\xC5\x99\x82\xa8\x9D\xDD\x2f\xF7\x24",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Vibe Coin",
        .unit = "VIBE",
        .contract_address =
            "\xe8\xff\x5c\x9c\x75\xde\xb3\x46\xac\xac\x49\x3c\x46\x3c\x89\x50\xbe\x03\xdf\xba",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "V-ID Token",
        .unit = "VIDT",
        .contract_address =
            "\x44\x5f\x51\x29\x9e\xf3\x30\x7d\xbd\x75\x03\x6d\xd8\x96\x56\x5f\x5b\x4b\xf7\xa5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VidyCoin",
        .unit = "VIDY",
        .contract_address =
            "\xc7\x7b\x23\x0f\x31\xb5\x17\xf1\xef\x36\x2e\x59\xc1\x73\xc2\xbe\x65\x40\xb5\xe8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VIEW",
        .unit = "VIEW",
        .contract_address =
            "\xf0\x3f\x8d\x65\xba\xfa\x59\x86\x11\xc3\x49\x51\x24\x09\x3c\x56\xe8\xf6\x38\xf0",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Vikky Token",
        .unit = "VIKKY",
        .contract_address =
            "\xd2\x94\x6b\xe7\x86\xf3\x5c\x3c\xc4\x02\xc2\x9b\x32\x36\x47\xab\xda\x79\x90\x71",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VIN",
        .unit = "VIN",
        .contract_address =
            "\xf3\xe0\x14\xfe\x81\x26\x78\x70\x62\x41\x32\xef\x3a\x64\x6b\x8e\x83\x85\x3a\x96",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ViteToken",
        .unit = "VITE",
        .contract_address =
            "\x1b\x79\x3e\x49\x23\x77\x58\xdb\xd8\xb7\x52\xaf\xc9\xeb\x4b\x32\x9d\x5d\xa0\x16",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Viuly Token",
        .unit = "VIU",
        .contract_address =
            "\x46\x4b\xad\xdc\xe9\xbd\x32\x58\x1a\x7d\x59\xd9\xbb\x83\x50\xc7\xc7\x76\x46\x68",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VALID",
        .unit = "VLD",
        .contract_address =
            "\x92\x2a\xc4\x73\xa3\xcc\x24\x1f\xd3\xa0\x04\x9e\xd1\x45\x36\x45\x2d\x58\xd7\x3c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VisionX",
        .unit = "VNX",
        .contract_address =
            "\x15\x63\xD5\x21\xba\x30\x9e\x2A\xd9\xf4\xaF\xfD\x6f\x4d\xE9\x75\x9E\x8d\x4F\x21",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VOISE",
        .unit = "VOISE",
        .contract_address =
            "\x83\xeE\xA0\x0D\x83\x8f\x92\xdE\xC4\xD1\x47\x56\x97\xB9\xf4\xD3\x53\x7b\x56\xE3",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Veros",
        .unit = "VRS",
        .contract_address =
            "\x92\xe7\x8d\xae\x13\x15\x06\x7a\x88\x19\xef\xd6\xdc\xa4\x32\xde\x9d\xcd\xe2\xe9",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "VeriSafe",
        .unit = "VSF",
        .contract_address =
            "\xac\x9c\xe3\x26\xe9\x5f\x51\xb5\x00\x5e\x9f\xe1\xdd\x80\x85\xa0\x1f\x18\x45\x0c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "vSlice",
        .unit = "VSL",
        .contract_address =
            "\x5c\x54\x3e\x7A\xE0\xA1\x10\x4f\x78\x40\x6C\x34\x0E\x9C\x64\xFD\x9f\xCE\x51\x70",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Vezt",
        .unit = "VZT",
        .contract_address =
            "\x97\x20\xb4\x67\xa7\x10\x38\x2A\x23\x2a\x32\xF5\x40\xbD\xCe\xd7\xd6\x62\xa1\x0B",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WABnetwork",
        .unit = "WAB",
        .contract_address =
            "\x4b\xbb\xc5\x7a\xf2\x70\x13\x8e\xf2\xff\x2c\x50\xdb\xfa\xd6\x84\xe9\xe0\xe6\x04",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WAX Token",
        .unit = "WAX",
        .contract_address =
            "\x39\xbb\x25\x9f\x66\xe1\xc5\x9d\x5a\xbe\xf8\x83\x75\x97\x9b\x4d\x20\xd9\x80\x22",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Wrapped BTC",
        .unit = "WBTC",
        .contract_address =
            "\x22\x60\xfa\xc5\xe5\x54\x2a\x77\x3a\xa4\x4f\xbc\xfe\xdf\x7c\x19\x3b\xc2\xc5\x99",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Winco",
        .unit = "WCO",
        .contract_address =
            "\xd4\x4b\xb6\x66\x39\x36\xca\xb1\x31\x05\x84\xa2\x77\xf7\xda\xa6\x94\x3d\x49\x04",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Webcoin",
        .unit = "WEB",
        .contract_address =
            "\x84\x0f\xe7\x5a\xbf\xad\xc0\xf2\xd5\x40\x37\x82\x95\x71\xb2\x78\x2e\x91\x9c\xe4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WELL Token",
        .unit = "WELL",
        .contract_address =
            "\x68\x4e\x2d\xcb\x12\xbb\x75\x52\x37\xe0\x72\x42\x52\x9c\x82\xf7\x8a\x84\xea\x61",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "We Show Token",
        .unit = "WET",
        .contract_address =
            "\x36\xd1\x0c\x68\x00\xd5\x69\xbb\x8c\x4f\xe2\x84\xa0\x5f\xfe\x3b\x75\x2f\x97\x2c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "W GREEN PAY",
        .unit = "WGP",
        .contract_address =
            "\xdd\x94\x84\x2c\x15\xab\xfe\x4c\x9b\xaf\xe4\x22\x2a\xde\x02\x89\x6b\xeb\x06\x4c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WaykiCoin",
        .unit = "WIC",
        .contract_address =
            "\x4f\x87\x8c\x08\x52\x72\x2b\x09\x76\xa9\x55\xd6\x8b\x37\x6e\x4c\xd4\xae\x99\xe5",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WILD Token",
        .unit = "WILD",
        .contract_address =
            "\xd3\xc0\x07\x72\xb2\x4d\x99\x7a\x81\x22\x49\xca\x63\x7a\x92\x1e\x81\x35\x77\x01",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WCOIN",
        .unit = "WIN",
        .contract_address =
            "\x89\x93\x38\xb8\x4d\x25\xac\x50\x5a\x33\x2a\xdc\xe7\x40\x2d\x69\x7d\x94\x74\x94",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WINGS",
        .unit = "WINGS",
        .contract_address =
            "\x66\x70\x88\xb2\x12\xce\x3d\x06\xa1\xb5\x53\xa7\x22\x1E\x1f\xD1\x90\x00\xd9\xaF",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "MyWish",
        .unit = "WISH",
        .contract_address =
            "\x1b\x22\xc3\x2c\xd9\x36\xcb\x97\xc2\x8c\x56\x90\xa0\x69\x5a\x82\xab\xf6\x88\xe6",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WIT",
        .unit = "WIT",
        .contract_address =
            "\xe1\x3e\xf2\x57\xcf\x4d\x5d\xf9\x28\xca\x11\xd2\x30\x42\x7c\x03\x76\x66\xd4\x66",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Wixlar",
        .unit = "WIX",
        .contract_address =
            "\x7b\xa1\x9b\x7f\x7d\x10\x6a\x9a\x1e\x09\x85\x39\x7b\x94\xf3\x8e\xee\x0b\x55\x5e",
        .decimals = 2,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "CrowdWizToken",
        .unit = "WIZ",
        .contract_address =
            "\x2f\x9b\x67\x79\xc3\x7d\xf5\x70\x72\x49\xee\xb3\x73\x4b\xbf\xc9\x47\x63\xfb\xe2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WPPTOKEN",
        .unit = "WPP",
        .contract_address =
            "\x05\x6d\xd2\x0b\x01\x79\x9e\x9c\x19\x52\xc7\xc9\xa5\xff\x44\x09\xa6\x11\x00\x85",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WePower",
        .unit = "WPR",
        .contract_address =
            "\x4C\xF4\x88\x38\x7F\x03\x5F\xF0\x8c\x37\x15\x15\x56\x2C\xBa\x71\x2f\x90\x15\xd4",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Worldcore",
        .unit = "WRC",
        .contract_address =
            "\x72\xad\xad\xb4\x47\x78\x4d\xd7\xab\x1f\x47\x24\x67\x75\x0f\xc4\x85\xe4\xcb\x2d",
        .decimals = 6,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Walton",
        .unit = "WTC",
        .contract_address =
            "\xb7\xcb\x1c\x96\xdb\x6b\x22\xb0\xd3\xd9\x53\x6e\x01\x08\xd0\x62\xbd\x48\x8f\x74",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Welltrado token",
        .unit = "WTL",
        .contract_address =
            "\x9a\x05\x87\xea\xe7\xef\x64\xb2\xb3\x8a\x10\x44\x2a\x44\xcf\xa4\x3e\xdd\x7d\x2a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WaleTokeN",
        .unit = "WTN",
        .contract_address =
            "\x0e\xa9\x84\xe7\x89\x30\x2b\x7b\x61\x21\x47\xe4\xe4\x14\x4e\x64\xf2\x14\x25\xeb",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WTT",
        .unit = "WTT",
        .contract_address =
            "\x84\x11\x9c\xb3\x3e\x8f\x59\x0d\x75\xc2\xd6\xea\x4e\x6b\x07\x41\xa7\x49\x4e\xda",
        .decimals = 0,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "wys Token",
        .unit = "WYS",
        .contract_address =
            "\xd8\x95\x0f\xde\xaa\x10\x30\x4b\x7a\x7f\xd0\x3a\x2f\xc6\x6b\xc3\x9f\x3c\x71\x1a",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "WaBi",
        .unit = "WaBi",
        .contract_address =
            "\x28\x6B\xDA\x14\x13\xa2\xDf\x81\x73\x1D\x49\x30\xce\x2F\x86\x2a\x35\xA6\x09\xfE",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "X8XToken",
        .unit = "X8X",
        .contract_address =
            "\x91\x0d\xfc\x18\xd6\xea\x3d\x6a\x71\x24\xa6\xf8\xb5\x45\x8f\x28\x10\x60\xfa\x4c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XACToken",
        .unit = "XAC",
        .contract_address =
            "\xde\x4c\x5a\x79\x19\x13\x83\x80\x27\xa2\x18\x57\x09\xe9\x8c\x5c\x60\x27\xea\x63",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Billionaire Token",
        .unit = "XBL",
        .contract_address =
            "\x49\xae\xc0\x75\x2e\x68\xd0\x28\x2d\xb5\x44\xc6\x77\xf6\xba\x40\x7b\xa1\x7e\xd7",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "BlitzPredict",
        .unit = "XBP",
        .contract_address =
            "\x28\xde\xe0\x1d\x53\xfe\xd0\xed\xf5\xf6\xe3\x10\xbf\x8e\xf9\x31\x15\x13\xae\x40",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ClearCoin",
        .unit = "XCLR",
        .contract_address =
            "\x1e\x26\xb3\xd0\x7e\x57\xf4\x53\xca\xe3\x0f\x7d\xdd\x2f\x94\x5f\x5b\xf3\xef\x33",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Data Transaction Token",
        .unit = "XD",
        .contract_address =
            "\x24\xdc\xc8\x81\xe7\xdd\x73\x05\x46\x83\x44\x52\xf2\x18\x72\xd5\xcb\x4b\x52\x93",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XinFin XDCE",
        .unit = "XDCE",
        .contract_address =
            "\x41\xab\x1b\x6f\xcb\xb2\xfa\x9d\xce\xd8\x1a\xcb\xde\xc1\x3e\xa6\x31\x5f\x2b\xf2",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Proxeus",
        .unit = "XES",
        .contract_address =
            "\xa0\x17\xac\x5f\xac\x59\x41\xf9\x50\x10\xb1\x25\x70\xb8\x12\xc9\x74\x46\x9c\x2c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ETERNAL TOKEN",
        .unit = "XET",
        .contract_address =
            "\x05\x4c\x64\x74\x1d\xba\xfd\xc1\x97\x84\x50\x54\x94\x02\x98\x23\xd8\x9c\x3b\x13",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Mixin",
        .unit = "XIN",
        .contract_address =
            "\xa9\x74\xc7\x09\xcf\xb4\x56\x66\x86\x55\x3a\x20\x79\x06\x85\xa4\x7a\xce\xaa\x33",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XCELTOKEN PLUS",
        .unit = "XLAB",
        .contract_address =
            "\x8c\x4e\x7f\x81\x4d\x40\xf8\x92\x9f\x91\x12\xc5\xd0\x90\x16\xf9\x23\xd3\x44\x72",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XMED Chain Token",
        .unit = "XMCT",
        .contract_address =
            "\x44\x44\x9F\xa4\xd6\x07\xF8\x07\xd1\xeD\x4a\x69\xad\x94\x29\x71\x72\x83\x91\xC8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XMAX",
        .unit = "XMX",
        .contract_address =
            "\x0f\x8c\x45\xb8\x96\x78\x4a\x1e\x40\x85\x26\xb9\x30\x05\x19\xef\x86\x60\x20\x9c",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Ink Protocol",
        .unit = "XNK",
        .contract_address =
            "\xbc\x86\x72\x7e\x77\x0d\xe6\x8b\x10\x60\xc9\x1f\x6b\xb6\x94\x5c\x73\xe1\x03\x88",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XENON",
        .unit = "XNN",
        .contract_address =
            "\xab\x95\xe9\x15\xc1\x23\xfd\xed\x5b\xdf\xb6\x32\x5e\x35\xef\x55\x15\xf1\xea\x69",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XOVBank",
        .unit = "XOV",
        .contract_address =
            "\x15\x3e\xd9\xcc\x1b\x79\x29\x79\xd2\xbd\xe0\xbb\xf4\x5c\xc2\xa7\xe4\x36\xa5\xf9",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XPlay",
        .unit = "XPA",
        .contract_address =
            "\x90\x52\x8a\xeb\x3a\x2b\x73\x6b\x78\x0f\xd1\xb6\xc4\x78\xbb\x7e\x1d\x64\x31\x70",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Rialto",
        .unit = "XRL",
        .contract_address =
            "\xb2\x47\x54\xbe\x79\x28\x15\x53\xdc\x1a\xdc\x16\x0d\xdf\x5c\xd9\xb7\x43\x61\xa4",
        .decimals = 9,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "XY Oracle",
        .unit = "XYO",
        .contract_address =
            "\x55\x29\x6f\x69\xf4\x0e\xa6\xd2\x0e\x47\x85\x33\xc1\x5a\x6b\x08\xb6\x54\xe7\x58",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Xaurum",
        .unit = "Xaurum",
        .contract_address =
            "\x4D\xF8\x12\xF6\x06\x4d\xef\x1e\x5e\x02\x9f\x1c\xa8\x58\x77\x7C\xC9\x8D\x2D\x81",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Yee - A Blockchain-powered & Cloud-based Socia",
        .unit = "YEE",
        .contract_address =
            "\x92\x21\x05\xfa\xd8\x15\x3f\x51\x6b\xcf\xb8\x29\xf5\x6d\xc0\x97\xa0\xe1\xd7\x05",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "YGGDRASH",
        .unit = "YEED",
        .contract_address =
            "\xca\x27\x96\xf9\xf6\x1d\xc7\xb2\x38\xaa\xb0\x43\x97\x1e\x49\xc6\x16\x4d\xf3\x75",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "YoloCash",
        .unit = "YLC",
        .contract_address =
            "\x21\xd5\x67\x8a\x62\xdf\xe6\x3a\x47\x06\x24\x69\xeb\xb2\xfa\xc2\x81\x7d\x88\x32",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "YOU Chain",
        .unit = "YOU",
        .contract_address =
            "\x34\x36\x4B\xEe\x11\x60\x7b\x19\x63\xd6\x6B\xCA\x66\x5F\xDE\x93\xfC\xA6\x66\xa8",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "YOYOW",
        .unit = "YOYOW",
        .contract_address =
            "\xcb\xea\xec\x69\x94\x31\x85\x7f\xdb\x4d\x37\xad\xdb\xbd\xc2\x0e\x13\x2d\x49\x03",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "YUP",
        .unit = "YUP",
        .contract_address =
            "\xd9\xa1\x2c\xde\x03\xa8\x6e\x80\x04\x96\x46\x98\x58\xde\x85\x81\xd3\xa5\x35\x3d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZAP TOKEN",
        .unit = "ZAP",
        .contract_address =
            "\x67\x81\xa0\xf8\x4c\x7e\x9e\x84\x6d\xcb\x84\xa9\xa5\xbd\x49\x33\x30\x67\xb1\x04",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZBToken",
        .unit = "ZB",
        .contract_address =
            "\xbd\x07\x93\x33\x2e\x9f\xb8\x44\xa5\x2a\x20\x5a\x23\x3e\xf2\x7a\x5b\x34\xb9\x27",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "0chain",
        .unit = "ZCN",
        .contract_address =
            "\xb9\xEF\x77\x0B\x6A\x5e\x12\xE4\x59\x83\xC5\xD8\x05\x45\x25\x8a\xA3\x8F\x3B\x78",
        .decimals = 10,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Zebi Coin",
        .unit = "ZCO",
        .contract_address =
            "\x20\x08\xe3\x05\x7b\xd7\x34\xe1\x0a\xd1\x3c\x9e\xae\x45\xff\x13\x2a\xbc\x17\x22",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZEON",
        .unit = "ZEON",
        .contract_address =
            "\xe5\xb8\x26\xca\x2c\xa0\x2f\x09\xc1\x72\x5e\x9b\xd9\x8d\x9a\x88\x74\xc3\x05\x32",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZeusNetwork",
        .unit = "ZEUS",
        .contract_address =
            "\xe7\xe4\x27\x9b\x80\xd3\x19\xed\xe2\x88\x98\x55\x13\x5a\x22\x02\x1b\xaf\x09\x07",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Zilliqa",
        .unit = "ZIL",
        .contract_address =
            "\x05\xf4\xa4\x2e\x25\x1f\x2d\x52\xb8\xed\x15\xe9\xfe\xda\xac\xfc\xef\x1f\xad\x27",
        .decimals = 12,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZINC",
        .unit = "ZINC",
        .contract_address =
            "\x4a\xac\x46\x1c\x86\xab\xfa\x71\xe9\xd0\x0d\x9a\x2c\xde\x8d\x74\xe4\xe1\xae\xea",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Zipper",
        .unit = "ZIP",
        .contract_address =
            "\xa9\xd2\x92\x7d\x3a\x04\x30\x9e\x00\x8b\x6a\xf6\xe2\xe2\x82\xae\x29\x52\xe7\xfd",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Zippie",
        .unit = "ZIPT",
        .contract_address =
            "\xed\xd7\xc9\x4f\xd7\xb4\x97\x1b\x91\x6d\x15\x06\x7b\xc4\x54\xb9\xe1\xba\xd9\x80",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Zilla",
        .unit = "ZLA",
        .contract_address =
            "\xfd\x89\x71\xd5\xe8\xe1\x74\x0c\xe2\xd0\xa8\x40\x95\xfc\xa4\xde\x72\x9d\x0c\x16",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZMINE Token",
        .unit = "ZMN",
        .contract_address =
            "\x55\x4f\xfc\x77\xf4\x25\x1a\x9f\xb3\xc0\xe3\x59\x0a\x6a\x20\x5f\x8d\x4e\x06\x7d",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Zenswap Network Token",
        .unit = "ZNT",
        .contract_address =
            "\x4f\xa0\x00\xdf\x40\xc0\x6f\xc8\xc7\xd9\x17\x96\x61\x53\x58\x46\xb7\xcd\x4f\x87",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZPER",
        .unit = "ZPR",
        .contract_address =
            "\xb5\xb8\xf5\x61\x6f\xe4\x2d\x5c\xec\xa3\xe8\x7f\x3f\xdd\xbd\xd8\xf4\x96\xd7\x60",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZRX",
        .unit = "ZRX",
        .contract_address =
            "\xe4\x1d\x24\x89\x57\x1d\x32\x21\x89\x24\x6d\xaf\xa5\xeb\xde\x1f\x46\x99\xf4\x98",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZeusShieldCoin",
        .unit = "ZSC",
        .contract_address =
            "\x7A\x41\xe0\x51\x7a\x5e\xcA\x4F\xdb\xC7\xFb\xeb\xA4\xD4\xc4\x7B\x9f\xF6\xDC\x63",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZTCoin",
        .unit = "ZT",
        .contract_address =
            "\xfe\x39\xe6\xa3\x2a\xcd\x2a\xf7\x95\x5c\xb3\xd4\x06\xba\x2b\x55\xc9\x01\xf2\x47",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ZeuxCoin",
        .unit = "ZUC",
        .contract_address =
            "\x6b\x46\x89\xe4\x51\x49\x57\x69\x9e\xde\xb2\xee\x91\xc9\x47\xf1\x8e\x43\x98\x06",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "0xcert Protocol Token",
        .unit = "ZXC",
        .contract_address =
            "\x83\xe2\xbe\x8d\x11\x4f\x96\x61\x22\x13\x84\xb3\xa5\x0d\x24\xb9\x6a\x56\x53\xf5",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "cVToken",
        .unit = "cV",
        .contract_address =
            "\xda\x6c\xb5\x8a\x0d\x0c\x01\x61\x0a\x29\xc5\xa6\x5c\x30\x3e\x13\xe8\x85\x88\x7c",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "DOW",
        .unit = "dow",
        .contract_address =
            "\x76\x97\x4c\x7b\x79\xdc\x8a\x6a\x10\x9f\xd7\x1f\xd7\xce\xb9\xe4\x0e\xff\x53\x82",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "eBitcoinCash",
        .unit = "eBCH",
        .contract_address =
            "\xaf\xc3\x97\x88\xc5\x1f\x0c\x1f\xf7\xb5\x53\x17\xf3\xe7\x02\x99\xe5\x21\xff\xf6",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "ETHGAS",
        .unit = "eGAS",
        .contract_address =
            "\xb5\x3a\x96\xbc\xbd\xd9\xcf\x78\xdf\xf2\x0b\xab\x6c\x2b\xe7\xba\xec\x8f\x00\xf8",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "fidentiaX",
        .unit = "fdX",
        .contract_address =
            "\x52\xa7\xcb\x91\x8c\x11\xa1\x69\x58\xbe\x40\xcb\xa7\xe3\x1e\x32\xa4\x99\xa4\x65",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "iEthereum",
        .unit = "iETH",
        .contract_address =
            "\x85\x9a\x9c\x0b\x44\xcb\x70\x66\xd9\x56\xa9\x58\xb0\xb8\x2e\x54\xc9\xe4\x4b\x4b",
        .decimals = 8,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "NucleusVision",
        .unit = "nCash",
        .contract_address =
            "\x80\x98\x26\xcc\xea\xb6\x8c\x38\x77\x26\xaf\x96\x27\x13\xb6\x4c\xb5\xcb\x3c\xca",
        .decimals = 18,
    },
    {
        .coin = ETHCoin_ETH,
        .name = "Synth sUSD",
        .unit = "sUSD",
        .contract_address =
            "\x57\xab\x1e\x02\xfe\xe2\x37\x74\x58\x0c\x11\x97\x40\x12\x9e\xac\x70\x81\xe9\xd3",
        .decimals = 18,
    },
};

const app_eth_erc20_params_t* app_eth_erc20_params_get(
    ETHCoin coin,
    const uint8_t* contract_address)
{
    for (size_t index = 0; index < sizeof(_erc20_params) / sizeof(app_eth_erc20_params_t);
         index++) {
        const app_eth_erc20_params_t* params = &_erc20_params[index];
        if (params->coin == coin &&
            MEMEQ(contract_address, params->contract_address, sizeof(params->contract_address))) {
            return params;
        }
    }
    return NULL;
}
