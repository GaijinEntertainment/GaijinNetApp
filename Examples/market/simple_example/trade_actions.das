require jsonrpc
require items_storage_db

require asset_entities
require base_entities
require market_utils



[jsonrpc_method]
def cst_set_unowned(assetid: uint64; amount: int; duplicate: bool)
  let assetidStr =  string(assetid)
  var onownedItem: Item

  if duplicate
    get(persist.transaction_history, get_transaction_id()) <| $(historyAssetInfo)
      onownedItem := historyAssetInfo.item

    if empty(onownedItem.id)
      send_error("Item was not found for duplicate transaction {get_transaction_id()}.")
      return

  else
    get(persist.items, assetidStr) <| $(item)
      onownedItem := item

    if empty(onownedItem.id)
      send_error("Item was not found : assetid {assetidStr}.")
      return

    let uintAmount = uint(amount)

    if amount <= 0 || uintAmount > onownedItem.count
      send_error("Wrong amount {string(amount)}.")
      return

    if onownedItem.count == uintAmount
      erase(persist.items, assetidStr)
    else
      onownedItem.id = generate_itemid()
      onownedItem.count = uintAmount

      persist.items[assetidStr].count -= uintAmount

    persist.transaction_history[get_transaction_id()] <- [[TransactionAssetInfo item=onownedItem]]

  send_result({{"itemid"=>onownedItem.id; "item_type"=>onownedItem.item_type}})


[jsonrpc_method]
def cst_set_owned(item_info: AssetClass; amount: int; price: string; duplicate: bool)
  var ownedItem: Item

  if duplicate
    get(persist.transaction_history, get_transaction_id()) <| $(historyAssetInfo)
      ownedItem := historyAssetInfo.item

    if empty(ownedItem.id)
      send_error("Item was not found for duplicate transaction {get_transaction_id()}.")
      return

  else
    if amount < 1
      send_error("Invalid amount")
      return

    if !is_marketable(item_info.item_type)
      send_error("Wrong item type. Can't to set owned the non-marketable item.")
      return

    ownedItem <- [[Item id=generate_itemid(), count=uint(amount), item_type=item_info.item_type]]

    persist.items[ownedItem.id] := ownedItem
    persist.transaction_history[get_transaction_id()] <- [[TransactionAssetInfo item=ownedItem, price=price]]

    add_items_to_db([{uint64 item_id_to_uint64(ownedItem.id)}])

  send_result({{"new_assetid"=>ownedItem.id}})
