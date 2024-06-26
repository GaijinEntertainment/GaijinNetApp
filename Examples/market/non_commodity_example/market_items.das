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
  ]];
  "thief_dagger" => [[AssetClassInfo
    market_hash_name = "Thief dagger",
    market_name = "Thief dagger",
    name = "Thief dagger",
    type_ = "Cold steel",
    background_color = "709100",
    name_color = "709100",
    icon_url = "https://store.gaijin.net/img/content/thief_dagger.png",
    market_fee = 0.1f,
    descriptions <- [{auto
      [[Description value="An ordinary dagger at first glance."]]
    }],
    tags <- [{ auto
      [[MarketTag
        name = "Legendary",
        category_name = "Quality",
        category = "quality",
        internal_name = "legendary",
        color = "F1C759"
      ]];
      [[MarketTag
        name = "Weapon",
        category_name = "Type",
        category = "type",
        internal_name = "weapon",
        color = "1C4C7A"
      ]]
    }],
    commodity = false,
    marketable = true
  ]]
}}



struct InfoModifier
  description: string
  icon_url: string



let item_upgrades <- {{
  "thief_dagger" => {{
    "poison_stone;diamond_coating" => [[InfoModifier
      description="Poison stone (+25 poison effect)\nDiamond coating (+15 bleed effect)",
      icon_url="https://store.gaijin.net/img/content/thief_dagger.png"
    ]];
    "ruby_gem" => [[InfoModifier
      description="Ruby gem (+25 fire damage effect)",
      icon_url="https://store.gaijin.net/img/content/thief_dagger_ruby.png"
    ]];
  }}
}}


let market_config_timestamp = 1719406620L
