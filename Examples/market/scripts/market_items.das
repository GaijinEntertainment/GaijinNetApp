require asset_entities



let market_items: table<string; AssetClassInfo> <- {{
  "big_health_potion" => [[AssetClassInfo
    market_hash_name = "Big health potion",
    market_name = "Big health potion",
    name = "Big health potion",
    type_ = "Potion",
    background_color = "3C352E",
    name_color = "2232D1",
    icon_url = "https://store.gaijin.net/img/content/big_health_potion.png",
    market_fee = 0.1f,
    descriptions <- [{auto
      [[Description value="Instantly restores full health bar."]]
    }],
    tags <- [{ auto
      [[MarketTag
        name = "Rare",
        category_name = "Quality",
        category = "quality",
        internal_name = "rare",
        color = "2232D1"
      ]];
      [[MarketTag
        name = "Support",
        category_name = "Type",
        category = "type",
        internal_name = "support",
        color = "23A639"
      ]]
    }],
    commodity = true,
    marketable = true
  ]]
}}


let market_config_timestamp = 1713265081L
