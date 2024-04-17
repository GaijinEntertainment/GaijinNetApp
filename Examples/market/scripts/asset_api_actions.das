require jsonrpc

require asset_entities
require base_entities
require market_utils



let shared
  DEFAULT_CONTEXT = "1"
  RESULT = "result"


[jsonrpc_method]
def get_contexts()
  var contexts: array<Context>
  emplace(contexts, [[Context id=DEFAULT_CONTEXT, name="default"]])

  for item in values(persist.items)
    if is_marketable(item)
      contexts[0].asset_count += 1u

  send_result(
    {{RESULT => [[GetContextResponse success=true, contexts <- contexts]]}}
  )


[jsonrpc_method]
def get_context_contents(contextid : string)
  if contextid != DEFAULT_CONTEXT
    send_error("Invalid context id")
    return

  var assets: array<Asset>

  for item in values(persist.items)
    if is_marketable(item)
      var itemClass <- [{auto [[ClassField name="item_type", value=item.item_type]]}]
      var asset <- [[Asset id=item.id, amount=item.count, class_ <- itemClass]]

      emplace(assets, asset)

  send_result(
    {{RESULT => [[AssetsResponse success=true, assets <- assets]]}}
  )



[jsonrpc_method]
def cst_get_asset_class(contextid : string; assets : array<string>)
  if contextid != DEFAULT_CONTEXT
    send_error("Invalid context id")
    return

  var assetsArr: array<Asset>

  for assetid in assets
    get(persist.items, assetid) <| $(item)
      if is_marketable(item)
        var itemClass <- [{auto [[ClassField name="item_type", value=item.item_type]]}]
        var asset <- [[Asset id=item.id, amount=item.count, class_ <- itemClass]]

        emplace(assetsArr, asset)

  send_result(
    {{RESULT => [[AssetsResponse success=true, assets <- assetsArr]]}}
  )


[jsonrpc_method]
def get_asset_class_info(language : string; class_ : table<string; string>)
  var assetClassInfo <- _get_asset_class_info(language, class_)

  if empty(assetClassInfo.market_hash_name)
    send_error("Cant find valid asset class info")
    return

  send_result(
    {{RESULT => [[GetAssetClassInfoResponse success=true, asset <- assetClassInfo]]}}
  )
