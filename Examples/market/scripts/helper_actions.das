require jsonrpc

require base_entities
require market_utils



[jsonrpc_method(create_profile)]
def get_profile()
  send_result(persist)


[jsonrpc_method]
def reinit_with_sample_items()
  clear(persist.items)
  var itemid <- generate_itemid()
  persist.items[itemid] <- [[Item item_type = "big_health_potion", id = itemid, count = 2u]]
  send_result(persist)


[jsonrpc_method]
def clear_transaction_history()
  clear(persist.transaction_history)
  send_result(persist)
