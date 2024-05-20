## Useful links

You can find the reference manual and sandbox links for daScript on the official website: [https://dascript.org/](https://dascript.org/)

The description of the Market API can be found here: [https://gaijinentertainment.github.io/GaijinApplicationPlatform/](https://gaijinentertainment.github.io/GaijinApplicationPlatform/)

## Overview

This example aims to demonstrate a possible implementation of scripts for organizing trading on the market. Here are some notes on this.

### base_entities

This module contains basic entities. They are not directly related to the market (except for TransactionAssetInfo), but they are likely to be present in some form when using the profile server.

The `Item` structure represents an abstract item. For the convenience of interacting with the market, let's consider it as a stack of items. In your implementation, this may differ. `item_type` represents a unique identifier for the type of items, which will be used in the asset API in the `class` object. Again, in your implementation, it may be something else, such as multiple fields. `id` is the unique identifier of a specific instance of the item.

The `TransactionAssetInfo` structure contains transaction data. In this example, it will be used when processing duplicate requests.

The `Profile` structure and its global instance `persist` contain user data and participate in almost all methods. The presence of `persist` is mandatory. However, its specific implementation is entirely up to you.

### helper_actions

This section contains helper methods for creating profiles and experiments.

`get_profile` creates a profile for the user (in case it's absent, other methods may return an error).

The names `reinit_with_sample_items`, `clear_items` and `clear_transaction_history` are self-explanatory.
Note that they are annotated with `admin_panel`. Actions with this annotation are displayed (and can be called) in the user management section on the dev-portal.

To use this methods you will need to set the headers:

- `token` - user token
- `appid` - your app (or `1197` for already deployed market example)
- `userid` (_optional_) - if you are operating with admin token with related permissions and want to update another profile (not token's owner)

The request is just simple:

```json
{
  "method": "das.get_profile",
  "id": "<some id>",
  "jsonrpc": "2.0",
  "params": {}
}
```

### utility_actions

These methods need to be implemented for correct interaction with the profile server.

`reinitProfile` will be automatically called when a player's profile is loaded from the database. Here, the player's items are actualized with database owners' storage (the mechanism is used to find the owner of the item).

`get_script_global_vars` is automatically called to check if there have been updates to the configuration of tradable items.

### market_items

This module reflects the key idea of organizing the storage of trading items in this example, namely - the file contains a table describing the items that can be sold or bought on the market. Also, here is the variable that should be updated when the market_items table changes. The keys of the table are the values of `item_type`.

When organizing trading in a real game, it will be more convenient to generate such a table, as well as localization data, automatically from some settings files. In any case, the specific implementation is up to you.

### market_utils

This module contains auxiliary functions for trading.

It is worth noting that in `generate_itemid`, the function `generate_unique_incremental_int` from the built-in `jsonrpc` module is used.

The asset API methods use the `is_marketable` function to determine if the user's item is marketable. `_get_asset_class_info` is used to return a complete description of the item in the `get_asset_class_info` method.

A good idea for experimenting with your own script implementation is to add localization here.

### Asset API

The `asset_entities` module implements asset API methods. For more detailed documentation, see the link above.

For simplicity, let's assume we have only one context. Therefore, all user items for which `is_marketable` returns `true` are considered to belong to the default context (see `get_contexts`).

The `asset_entities` module contains classes to support this API (forming a response etc). Since some API fields coincide with daScript keywords (class, type), which cannot be used directly, a special annotation `serialize_as` is used to transform the field name in the response to the specified one.

To send errors, the `send_error` method function from the built-in `jsonrpc` module is used. To send a successful result, the `send_result` function is used. Note that after `send_error`, you can no longer use the `send_result` function (which is obviously) - this will result in an exception.

### trade_actions

These methods are directly responsible for assigning and removing user items. To support transaction duplicates, the `get_transaction_id` function from the `jsonrpc` module is used. For a repeated transaction, it will be the same. For simplicity, transaction history clearing is not performed.
