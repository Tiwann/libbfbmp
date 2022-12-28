newoption {
    trigger     = "lib",
    description = "Static or Shared library",
    default     = "shared",
    allowed = {
       { "static",    "Static Library" },
       { "shared",  "Shared Library" },
    }
 }