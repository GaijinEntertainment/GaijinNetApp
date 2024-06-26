require jsonrpc
require daslib/strings_boost

require asset_entities
require base_entities
require market_items



let
  INVALID_ITEM_ID = 0UL


def is_marketable(item: Item): bool
  return key_exists(market_items, item.item_type)


def is_marketable(item_type: string): bool
  return key_exists(market_items, item_type)


def generate_itemid(): string
  return string(generate_unique_incremental_int())


def get_market_config_timestamp(): int64
  return market_config_timestamp


def item_to_class(item: Item): array<ClassField>
  var class_: array<ClassField>

  emplace(class_, [[ClassField name="item_type", value=item.item_type]])

  if !empty(item.upgrades)
    emplace(class_, [[ClassField name="upgrades", value=item.upgrades]])

  return <- class_

def _get_asset_class_info(language: string; class_ : table<string; string>): AssetClassInfo
  var itemType: string
  var result: AssetClassInfo

  get(class_, "item_type") <| $(itemType_)
    itemType = itemType_

    get(market_items, itemType) <| $(assetClassInfo)
      result := assetClassInfo

      get(class_, "upgrades") <| $(itemUpgrades)
        var foundUpgrades = false
        get(item_upgrades, itemType) <| $(upgradesTable)
          get(upgradesTable, itemUpgrades) <| $(upgradeInfo)
            result.icon_url = upgradeInfo.icon_url
            emplace(result.descriptions, [[Description value=upgradeInfo.description]])
            foundUpgrades = true

        if !foundUpgrades
          result <- [[AssetClassInfo]] // reset the result because upgrades was not found

  // Here can be resolved all localizations (name, type, description, tags, etc) with passed language code
  // in a custom way. To simplify, we just use basic English localization.

  return <- result


def item_id_to_uint64(item_id_str : string) : uint64
  let item_id : uint64 = to_uint64(item_id_str)
  // conversion from invalid string (e.g. "jklp") doesn't raise an exception
  if string(item_id) == item_id_str
    return item_id
  else
    return INVALID_ITEM_ID
