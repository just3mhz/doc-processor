#include "processor.h"


DocumentProcessor::DocumentProcessor(std::unique_ptr<Storage> storage)
    : storage_(std::move(storage))
{}


std::shared_ptr<Document> DocumentProcessor::Process(std::shared_ptr<Document> document)
{
    // TODO: Maybe too slow
    std::lock_guard lock(mutex_);

    if (!document) {
        return nullptr;
    }

    auto savedDocument = storage_->get(document->url);

    if (!savedDocument) {
        document->firstFetchTime = document->fetchTime;
        storage_->update(document);
        return document;
    }

    if (document->fetchTime == savedDocument->fetchTime) {
        document->firstFetchTime = savedDocument->firstFetchTime;
        return document;
    }

    if (document->fetchTime > savedDocument->fetchTime) {
        savedDocument->text = std::move(document->text);
        savedDocument->fetchTime = document->fetchTime;
    }

    if (document->fetchTime < savedDocument->fetchTime) {
        if (savedDocument->firstFetchTime > document->fetchTime) {
            savedDocument->firstFetchTime = document->fetchTime;
            savedDocument->pubDate = document->pubDate;
        }
    }

    storage_->update(savedDocument);
    return savedDocument;
}
