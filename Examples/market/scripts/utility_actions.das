require jsonrpc
require items_storage_db
require logger

require base_entities
require market_utils



[jsonrpc_method]
def reinitProfile()
  LOGD("Reinit profile")
  var actualItems: array<uint64>

  for item in values(persist.items)
    if is_marketable(item) && item.count > 0u
      let itemId: uint64 = item_id_to_uint64(item.id)
      if itemId != INVALID_ITEM_ID
        actualItems |> push(itemId)

  actualize_items(actualItems)


[jsonrpc_method(internal)]
def get_script_global_vars()
  send_result(
    {{"market_config_timestamp" => get_market_config_timestamp()}}
  )
