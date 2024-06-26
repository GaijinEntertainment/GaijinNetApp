struct Context
  id: string
  name: string
  asset_count: uint


struct ClassField
  name: string
  value: string


struct Asset
  id: string
  amount: uint
  [[serialize_as ="class"]] class_: array<ClassField>


struct MarketTag
  name: string
  category_name: string
  category: string
  internal_name: string
  color: string


struct Description
  value: string


struct AssetClass
  item_type : string
  upgrades: string


struct AssetClassInfo
  market_hash_name: string
  market_name: string
  name: string
  [[serialize_as ="type"]] type_: string
  background_color: string
  name_color: string
  icon_url: string
  market_fee: float
  descriptions: array <Description>
  tags: array<MarketTag>
  commodity: bool
  marketable: bool


struct GetContextResponse
  success: bool
  contexts: array<Context>


struct AssetsResponse // the same for get_context_contents and cst_get_asset_class
  success: bool
  assets : array<Asset>


struct GetAssetClassInfoResponse
  success: bool
  asset: AssetClassInfo
