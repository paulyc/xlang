﻿# WARNING: Please don't edit this file. It was generated by Python/WinRT

from pyrt import _import_ns
import typing
import enum

__ns__ = _import_ns("Windows.Storage.Search")

try:
    import pyrt.windows.data.text
except:
    pass

try:
    import pyrt.windows.foundation
except:
    pass

try:
    import pyrt.windows.foundation.collections
except:
    pass

try:
    import pyrt.windows.storage
except:
    pass

try:
    import pyrt.windows.storage.fileproperties
except:
    pass

try:
    import pyrt.windows.storage.streams
except:
    pass

class CommonFileQuery(enum.IntEnum):
    DefaultQuery = 0
    OrderByName = 1
    OrderByTitle = 2
    OrderByMusicProperties = 3
    OrderBySearchRank = 4
    OrderByDate = 5

class CommonFolderQuery(enum.IntEnum):
    DefaultQuery = 0
    GroupByYear = 100
    GroupByMonth = 101
    GroupByArtist = 102
    GroupByAlbum = 103
    GroupByAlbumArtist = 104
    GroupByComposer = 105
    GroupByGenre = 106
    GroupByPublishedYear = 107
    GroupByRating = 108
    GroupByTag = 109
    GroupByAuthor = 110
    GroupByType = 111

class DateStackOption(enum.IntEnum):
    NONE = 0
    Year = 1
    Month = 2

class FolderDepth(enum.IntEnum):
    Shallow = 0
    Deep = 1

class IndexedState(enum.IntEnum):
    Unknown = 0
    NotIndexed = 1
    PartiallyIndexed = 2
    FullyIndexed = 3

class IndexerOption(enum.IntEnum):
    UseIndexerWhenAvailable = 0
    OnlyUseIndexer = 1
    DoNotUseIndexer = 2
    OnlyUseIndexerAndOptimizeForIndexedProperties = 3

ContentIndexer = __ns__.ContentIndexer
ContentIndexerQuery = __ns__.ContentIndexerQuery
IndexableContent = __ns__.IndexableContent
QueryOptions = __ns__.QueryOptions
SortEntryVector = __ns__.SortEntryVector
StorageFileQueryResult = __ns__.StorageFileQueryResult
StorageFolderQueryResult = __ns__.StorageFolderQueryResult
StorageItemQueryResult = __ns__.StorageItemQueryResult
StorageLibraryChangeTrackerTriggerDetails = __ns__.StorageLibraryChangeTrackerTriggerDetails
StorageLibraryContentChangedTriggerDetails = __ns__.StorageLibraryContentChangedTriggerDetails
ValueAndLanguage = __ns__.ValueAndLanguage
IIndexableContent = __ns__.IIndexableContent
IStorageFolderQueryOperations = __ns__.IStorageFolderQueryOperations
IStorageQueryResultBase = __ns__.IStorageQueryResultBase
SortEntry = __ns__.SortEntry
